#version 430

// mathematical constants
#define M_PI 3.1415926535898
#define M_PI_2 1.57079632679
#define SQRT_3 1.732050808
#define HALF_ROOT_2 0.70710676908493042

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout (std430, binding = 1) buffer rasterBuffer { float rasters []; };            // array of input rasters for convolutions

// currently, these are all equirectangular grids 2x wide by x high where x is a power of two.
// Output and all input grids are to cover the same bounds at the same resolution.
layout (std430, binding = 0) buffer heightMapBuffer { float height_map_out []; };

uniform vec4 bounds = vec4 (-M_PI, -M_PI / 2, M_PI, M_PI / 2);

// indices to faces of the cube map


#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_NORTH 2
#define FACE_SOUTH 3
#define FACE_EAST 4
#define FACE_WEST 5

// cubemap parameter
uniform int size;


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

float hash (float n) {
    return fract (sin (n) * 43758.5453);
}

// Cubic spline: y = x^2 (3 - 2x) = 3x^2 - 2x^3
float SCurve3 (float x) {
    return (x * x * (3.0 - 2.0 * x));
}

// Quintic spline: y = 6x^5 - 15x^4 + 10x^3
float SCurve5 (float x) {
    float x3 = x * x * x;
    float x4 = x3 * x;
    float x5 = x4 * x;
    return (6.0 * x5) - (15.0 * x4) + (10.0 * x3);
}

/// Cubic interpolation with four controls (from libnoise).
/// The alpha value should range from 0.0 to 1.0.  If the alpha value is
/// 0.0, this function returns @a n1.  If the alpha value is 1.0, this
/// function returns @a n2.
float cubicInterpolate (float n0, float n1, float n2, float n3, float a) {
    float p = (n3 - n2) - (n0 - n1);
    float q = (n0 - n1) - p;
    float r = n2 - n0;
    float s = n1;
    return p * a * a * a + q * a * a + r * a + s;
}

// Find the x and y coordinates for a geolocation in a raster of height and width given by size and
// spanning the given bounds. If the geolocation is outwith the bounds the returned index will be outwith
// the raster size, so check for this before trying to use the index.
ivec2 index (vec2 g, vec4 bounds, vec2 size) {
    vec2 a = bounds.xy;
    vec2 b = bounds.zw;
    if (a.x > b.x) { // if this is TRUE the raster bounds straddle the dateline
        if (g.x < a.x) {
            g.x += M_PI * 2;
        }
        b.x += M_PI * 2;
    }

    float rx = (g.x - a.x) / (b.x - a.x);
    float ry = (g.y - a.y) / (b.y - a.y);
    return ivec2 (rx * size.x, ry * size.y);
}

// Coordinate systems:
//      screen coordinates (corresponds to GlobalInvoicationID) - the coordinates of the texel being rendered. 0 <= x <= resolution * 2; 0 <= x <= resolution.
//      map coordinates - the coordinates of a point on the logical map. -2 * M_PI < x <= 2 * M_PI; -M_PI <= y <= M_PI.
//          (the logical map is the equirectangular projection with scale 1 unit = 1 radian of latitude or longitude).
//      geolocation - the latitude and longitude (in that order) of a point on the geoid under the reigning projection. -2 * M_PI < x <= 2 * M_PI; -M_PI <= y <= M_PI.
//      cartesian - the point in 3D space corresponding to a geolocation on the surface of the (spherical) geoid. -1 <= x, y, z <= 1 and x^2 + y^2 + z^2 = 1.

// Returns the map coordinates for a given GlobalInvocationID (essentially screen coordinates) taking into account the scale.
// to do - deal with bounds crossing dateline; set bounds via uniform variable; map bounds to globe in the rendering shader

vec2 mapPos (ivec2 pos) {
    vec2 a = bounds.xy;
    vec2 b = bounds.zw;
    if (a.x > b.x) { // if this is TRUE the raster bounds straddle the dateline
        b.x += M_PI * 2;
    }

    float ix = pos.x / size;
    float iy = pos.y / size;
    vec2 g = vec2 ((b.x - a.x) * ix, (b.y - a.y) * iy);
    g += a;
    if (g.x > M_PI) { g.x -= M_PI* 2; }
    return g;
}

// Returns the screen coordinates (GlobalInvocationID) for given map coordinates taking into account the scale.

ivec2 scrPos (vec2 g) {
    vec2 j = g;
    j /= M_PI;
    j.x = (j.x + 1) * size;;
    j.y = (j.y + 0.5) * size;;
    return ivec2 (j);
}


vec4 toGreyscale (vec4 color) {
    float l = sqrt (color.x * color.x + color.y * color.y + color.z * color.z);
    return vec4 (l, l, l, 1.0);
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

bool inBounds (vec2 g) {
    vec2 a = bounds.xy;
    vec2 b = bounds.zw;
    if (a.x > b.x) { // if this is TRUE the raster bounds straddle the dateline
        if (g.x < a.x) {
            g.x += M_PI * 2;
        }
        b.x += M_PI * 2;
    }
    return (a.x < g.x && a.y < g.y && b.x > g.x && b.y > g.y);
}


// inserted code //

void main() {
        int i = (gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_WorkGroupID.z);
        vec3 c = indexToCartesian (pos);
        vec2 g = toGeolocation (c);
        uint i = pos.z * size * size + pos.y * size + pos.x;

        float v = value (c, g);
        height_map_out [i] = v;
}
