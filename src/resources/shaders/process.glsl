#version 120

// mathematical constants
#define M_PI 3.1415926535898
#define M_PI_2 1.57079632679
#define SQRT_3 1.732050808
#define HALF_ROOT_2 0.70710676908493042

layout (local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;
layout (std430, binding = 1) buffer rasterBuffer { float rasters; };            // array of input rasters for GRIDS, 4 cubic spheres
// this is the cubic sphere into which the output will be written
layout (std430, binding = 0) buffer heightMapBuffer { float height_map_out []; };



// indices to faces of the cube map


#define FACE_FRONT 4
#define FACE_BACK 5
#define FACE_NORTH 2
#define FACE_SOUTH 3
#define FACE_EAST 0
#define FACE_WEST 1



// Neighbouring panels for each panel, in the order +x, -x, +y, -y
const uint ADJACENT [6] [4] = { { 4, 5, 2, 3 }, { 4, 5, 2, 3, }, { 4, 5, 0, 1 }, { 4, 5, 0, 1 }, { 2, 3, 0, 1 }, { 2, 3, 0, 1 } };

ivec3 [4] adjacent (ivec3 fuv) {
    ivec3 [4] m;

    m[0] = ivec3 (fuv.x + 1, fuv.y, fuv.z);
    m[1] = ivec3 (fuv.x - 1, fuv.y, fuv.z);
    m[2] = ivec3 (fuv.x, fuv.y + 1, fuv.z);
    m[3] = ivec3 (fuv.x, fuv.y - 1, fuv.z);

    if (m[0].x < 0)         { m[0].x = 0;           m[0].z = int (ADJACENT [fuv.z] [0]); }
    if (m[1].x > size - 1)  { m[1].x = size - 1;    m[0].z = int (ADJACENT [fuv.z] [1]); }
    if (m[2].y < 0)         { m[2].y = 0;           m[2].z = int (ADJACENT [fuv.z] [2]); }
    if (m[3].y > size - 1)  { m[3].y = size - 1;    m[3].z = int (ADJACENT [fuv.z] [3]); }

    return m;
}

ivec3 [3] [3] surrounding (ivec3 fuv) {
    ivec3 [4] adj = adjacent (fuv);
    ivec3 [3] [3] k;
    k [0] [0] = ivec3 (adj [1].x, adj [3].y, adj [1].z == fuv.z ? (adj [3].z == fuv.z ? fuv.z : adj [3].z) : adj [1].z);
    k [0] [1] = ivec3 (adj [1].x, fuv.y, adj [1].z);
    k [0] [2] = ivec3 (adj [1].x, adj [2].y, adj [1].z == fuv.z ? (adj [2].z == fuv.z ? fuv.z : adj [2].z) : adj [1].z);
    k [1] [0] = ivec3 (fuv.x, adj [3].y, adj [3].z);
    k [1] [1] = ivec3 (fuv.x, fuv.y, fuv.z);
    k [1] [2] = ivec3 (fuv.x, adj [2].y, adj [2].z);
    k [0] [0] = ivec3 (adj [0].x, adj [3].y, adj [0].z == fuv.z ? (adj [3].z == fuv.z ? fuv.z : adj [3].z) : adj [0].z);
    k [0] [0] = ivec3 (adj [0].x, fuv.y, adj [0].z);
    k [0] [0] = ivec3 (adj [0].x, adj [2].y, adj [0].z == fuv.z ? (adj [2].z == fuv.z ? fuv.z : adj [2].z) : adj [0].z);
    return k;
}


vec3 toCartesian (in vec2 geolocation) { // x = longitude, y = latitude
    vec3 cartesian;
    cartesian.x = cos (geolocation.x) * cos (geolocation.y);
    cartesian.z = cos (geolocation.y) * sin (geolocation.x);
    cartesian.y = sin (geolocation.y);
    return cartesian;
}

vec2 toGeolocation (vec3 cartesian) {
    float phi = atan (cartesian.z, cartesian.x);
    float theta = acos (cartesian.y) - (M_PI / 2);
    return vec2 (phi, -theta);
}


float distance (vec2 loc1, vec2 loc2, float range) {
    float s = acos (sin (loc1.x) * sin (loc2.x) + cos (loc1.x) * cos (loc2.x) * cos (abs (loc1.y - loc2.y)));
    return mix (1.0, -1.0, s / range);
}

// convert a cubemap index to a cartesian vector
// the cubemap index consists of x and y = cell's 2D coordinates on its face and z = the index of the face on which the cell sits
vec3 indexToCartesian (ivec3 fuv) {
    vec2 uv = vec2 ((float (fuv.x) / float (size)) * 2.0f - 1.0f,
    (float (fuv.y) / float (size)  * 2.0f - 1.0f));

    float x, y, z;      // cartesian coordinates on the cube
    if (fuv.z == FACE_NORTH)  { y =  1.0; x = uv.x; z = uv.y; }
    if (fuv.z == FACE_SOUTH)  { y = -1.0; x = uv.x; z = uv.y; }
    if (fuv.z == FACE_EAST)   { x =  1.0; y = uv.x; z = uv.y; }
    if (fuv.z == FACE_WEST)   { x = -1.0; y = uv.x; z = uv.y; }
    if (fuv.z == FACE_FRONT)  { z =  1.0; x = uv.x; y = uv.y; }
    if (fuv.z == FACE_BACK)   { z = -1.0; x = uv.x; y = uv.y; }
    float dx = x * sqrt (1.0f - y * y * 0.5f - z * z * 0.5f + (y * y * z * z) / 3.0f);
    float dy = y * sqrt (1.0f - z * z * 0.5f - x * x * 0.5f + (z * z * x * x) / 3.0f);
    float dz = z * sqrt (1.0f - x * x * 0.5f - y * y * 0.5f + (x * x * y * y) / 3.0f);
    return vec3 (dx, dy, dz);
}


ivec3 cartesianToIndex (vec3 cartesian) {
    vec3 position = cartesian;
    ivec3 fuv;
    float x = position.x, y = position.y, z = position.z;
    float fx = abs (x), fy = abs (y), fz = abs (z);

    if (fy >= fx && fy >= fz) {
        float a2 = x * x * 2.0;
        float b2 = z * z * 2.0;
        float inner = -a2 + b2 - 3;
        float innersqrt = -sqrt ((inner * inner) - 12.0 * a2);
        position.x = (x == 0.0 || x == -0.0) ? 0.0 : (sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2);
        position.z = (z == 0.0 || z == -0.0) ? 0.0 : (sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2);
        if (position.x > 1.0) { position.x = 1.0; }
        if (position.z > 1.0) { position.z = 1.0; }
        if (x < 0) { position.x = -position.x; }
        if (z < 0) { position.z = -position.z; }
        fuv.z = (y > 0) ? FACE_NORTH : FACE_SOUTH;;
        fuv.x = int ((position.x * 0.5 + 0.5) * size);
        fuv.y = int ((position.z * 0.5 + 0.5) * size);
    } else if (fx >= fy && fx >= fz) {
        float a2 = y * y * 2.0;
        float b2 = z * z * 2.0;
        float inner = -a2 + b2 - 3;
        float innersqrt = -sqrt ((inner * inner) - 12.0 * a2);
        position.y = (y == 0.0 || y == -0.0) ? 0.0 : (sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2);
        position.z = (z == 0.0 || z == -0.0) ? 0.0 : (sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2);
        if (position.y > 1.0) { position.y = 1.0; }
        if (position.z > 1.0) { position.z = 1.0; }
        if (y < 0) { position.y = -position.y; }
        if (z < 0) { position.z = -position.z; }
        fuv.z = (x > 0) ? FACE_EAST : FACE_WEST;
        fuv.x = int ((position.y * 0.5 + 0.5) * size);
        fuv.y = int ((position.z * 0.5 + 0.5) * size);
    } else {
        float a2 = x * x * 2.0;
        float b2 = y * y * 2.0;
        float inner = -a2 + b2 - 3;
        float innersqrt = -sqrt ((inner * inner) - 12.0 * a2);
        position.x = (x == 0.0 || x == -0.0) ? 0.0 : (sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2);
        position.y = (y == 0.0 || y == -0.0) ? 0.0 : (sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2);
        if (position.x > 1.0) { position.x = 1.0; }
        if (position.y > 1.0) { position.y = 1.0; }
        if (x < 0) { position.x = -position.x; }
        if (y < 0) { position.y = -position.y; }
        fuv.z = (z > 0) ? FACE_FRONT :  FACE_BACK;
        fuv.x = int ((position.x * 0.5 + 0.5) * size);
        fuv.y = int ((position.y * 0.5 + 0.5) * size);

    }
    return fuv;
}


// inserted code //

void main() {
    //  iteration sequence number is gl_GlobalInvocationID.x


    height_map_out [i] = value (pos, c, g);
}


// debugging outputs
//height_map_out [i] = (float (pos.z) /  3.0) - 1.0;   // face
//height_map_out [i] = (float (pos.y) /  size) * 2.0 - 1.0;  // y-coordinate
//height_map_out [i] = (float (pos.x) /  size) * 2.0 - 1.0;  // x-coordinate
