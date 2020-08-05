#version 430

// mathematical constants
#define M_PI 3.1415926535898
#define M_PI_2 1.57079632679
#define SQRT_3 1.732050808
#define HALF_ROOT_2 0.70710676908493042

layout (local_size_x = 32, local_size_y = 32) in;
layout (rgba32f, binding = 7) uniform image2D destTex;                               // output texture
layout (std430, binding = 5) buffer inBUffer { float height_map_in []; };            // array of input textures for modules that require them
layout (std430, binding = 6) buffer colorMapBuffer { vec4 color_map_in []; };        // legend

uniform int colorMapBufferSize;
uniform int imageHeight = 512;


uniform vec4 bounds = vec4 (-M_PI, -M_PI / 2, M_PI, M_PI / 2);
uniform vec4 defaultColor;

// first element is the datum longitude, second element is the datum latitude, third element is the scale
uniform vec3 datum;



// indices to faces of the cube map

#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_NORTH 2
#define FACE_SOUTH 3
#define FACE_EAST 4
#define FACE_WEST 5

// projection types
const int PROJ_EQUIRECTANGULAR = 0;
const int PROJ_MERCATOR = 1;
const int PROJ_ORTHOGRAPHIC = 2;
//...

// selected projection for main map
// this is ignored for the inset pass, which is always equirectnagular
uniform int projection = PROJ_ORTHOGRAPHIC;

// output identifiers
const int MODE_PREVIEW = 0;
const int MODE_OVERVIEW = 1;
const int MODE_GLOBE = 2;

uniform int mode = MODE_GLOBE;

// statistics
int hypsographyResolution;
float minAltitude = 0;
float maxAltitude = 0;

// cubemap parameters
uniform int size;

vec3 toCartesian (in vec2 geolocation) { // x = longitude, y = latitude
    vec3 cartesian;
    cartesian.x = cos (geolocation.x) * cos (geolocation.y);
    cartesian.z = cos (geolocation.y) * sin (geolocation.x);
    cartesian.y = sin (geolocation.y);

    //cartesian.w = geolocation.z;
    return cartesian;
}

vec2 toGeolocation (vec3 cartesian) {
    float phi = atan (cartesian.z, cartesian.x);
    float theta = acos (cartesian.y) - (M_PI / 2);
    return vec2 (phi, -theta);
}

//
// Find the colour in the current legend corresponding to the given noise value. This works the same way as map, above.
vec4 findColor (float value) {
    value = value / 2 + 0.5;
    float index = value * colorMapBufferSize;
    int indexPos = int (index);
    int index0 = clamp (indexPos    , 0, colorMapBufferSize - 1);
    int index1 = clamp (indexPos + 1, 0, colorMapBufferSize - 1);

    vec4 out0 = color_map_in [index0];
    vec4 out1 = color_map_in [index1];
    float alpha = (index - index0) / (index1 - index0);
    return out0;
    //return mix (vec4 (out0.xyz, 1.0), vec4 (out1.xyz, 1.0), alpha);
}

// raster constrained to bounds (a.x, a.y) - (b.x, b.y)
float lookup (vec2 g) {
    vec2 a = bounds.xy;
    vec2 b = bounds.zw;
    if (a.x > b.x) { // if this is TRUE the raster bounds straddle the dateline
        if (g.x < a.x) {
            g.x += M_PI * 2;
        }
        b.x += M_PI * 2;
    }

    float rx = (g.x - a.x) / (b.x - a.x) * size;
    float ry = (g.y - a.y) / (b.y - a.y) * size;
    int index = int (ry * size + rx);
    return height_map_in [index];
}



// Coordinate systems:
//      screen coordinates (corresponds to GlobalInvoicationID) - the coordinates of the texel being rendered. 0 <= x <= resolution * 2; 0 <= x <= resolution.
//      map coordinates - the coordinates of a point on the logical map. -2 * M_PI < x <= 2 * M_PI; -M_PI <= y <= M_PI.
//          (the logical map is the equirectangular projection with scale 1 unit = 1 radian of latitude or longitude).
//      geolocation - the latitude and longitude (in that order) of a point on the geoid under the reigning projection. -2 * M_PI < x <= 2 * M_PI; -M_PI <= y <= M_PI.
//      cartesian - the point in 3D space corresponding to a geolocation on the surface of the (spherical) geoid. -1 <= x, y, z <= 1 and x^2 + y^2 + z^2 = 1.

// Returns the map coordinates for a given GlobalInvocationID (essentially screen coordinates) taking into account the scale.
vec2 mapPos (vec2 pos, bool inset) {
    vec2 j = vec2 ((pos.x - imageHeight) / (imageHeight * 2), (pos.y / 2 - (imageHeight / 4)) / imageHeight);
    j *= M_PI * 2;
    j *= inset ? 1.0 : datum.z;
    return j;
}

vec2 mapPos (vec2 pos) {
    return mapPos (pos, false);
}

// Returns the screen coordinates (GlobalInvocationID) for given map coordinates taking into account the scale.

ivec2 scrPos (vec2 g, bool inset) {
    vec2 j = g / (inset ? 1.0 : datum.z);
    j /= M_PI;
    j.x = (j.x + 1) * imageHeight;
    j.y = (j.y + 0.5) * imageHeight;
    return ivec2 (j);
}

ivec2 scrPos (vec2 g) {
    return scrPos (g, false);
}

    // Projections.
    // one "unit" of i here on the flat map will now be one planetary radius if the projection is equirectangular.
    // branches to select projection are OK because all invocations will be using the same projection (except for texels in the inset area).

    // Forward projection to get the screen coordinate corresponding to the by the given geolocation under the current projection.
    // The first two components in the returned vector are x and y; the third is logical distance from the
    // centre of projection. Logic here should be identical to that in the corresponding implementations of
    // calenhad::mapping::projection::Projection::forward.

vec3 forward (vec2 g) {

    int p = projection;
    vec2 d = datum.xy;

    if (p == PROJ_ORTHOGRAPHIC) {
        float x = cos (g.y) * sin (g.x - d.x);
        float y = cos (d.y) * sin (g.y) - sin (d.y) * cos (g.y) * cos (g.x - d.x);
        float rho = sin (d.y) * sin (g.y) + cos (d.y) * cos (g.y) * cos (g.x - d.x);
        return vec3 (x, y, rho);
    }

    if (p == PROJ_MERCATOR) {
        float y = log (tan ((M_PI / 4) + (g.y / 2)));
        float x = g.x - d.x;
        vec2 i = vec2 (x, y);
        return vec3 (i.xy, abs (g.y / (M_PI / 2)));
    }

    if (p == PROJ_EQUIRECTANGULAR) {
        vec2 i = vec2 ((g.x - d.x) * cos (d.y), g.y);
        float visible = abs (g.y / (M_PI / 2));
        return vec3 (i.xy, visible);
    }

    return vec3 (0.0, 0.0, -1.0);
}

// Inverse projection to get the geolocation represented  by the given position under the current projection.
// The first two components in the returned vector are longitude and latitude; the third is logical distance from the
// centre of projection. Logic here should be identical to that in the corresponding implementations of
// calenhad::mapping::projection::Projection::inverse.

vec3 inverse (vec2 i) {

    // use the set values for the inset when we are composing the inset, otherwise the reigning parameters for the main map.
    int p = projection;
    vec2 d = datum.xy;

    if (p == PROJ_ORTHOGRAPHIC) {
        float x = i.x;
        float y = i.y;
        float rho = sqrt (x * x + y * y);
        float c = asin (rho);
        float lat = asin (cos (c) * sin (d.y) + (((y * sin (c)) * cos (d.y) / rho)));
        float lon = d.x + atan ((x * sin (c)), (rho * cos (d.y) * cos (c) - y * sin (d.y) * sin (c)));
        float visible = rho;
        return vec3 (lon, lat, visible);
    }

    if (p == PROJ_MERCATOR) {
        float lat = M_PI / 2 - 2 * atan (exp (-i.y));
        float lon = i.x + d.x;
        vec2 g = vec2 (lon, lat);
        return vec3 (g.xy, abs (g.y / (M_PI / 2)));
    }


    if (p == PROJ_EQUIRECTANGULAR) {
        vec2 g = vec2 (i.x + d.x, i.y);
        float visible = abs (g.y / (M_PI / 2));
        return vec3 (g.xy, visible);
    }



    return vec3 (0.0, 0.0, -1.0);

}

vec4 toGreyscale (vec4 color) {
    float l = sqrt (color.x * color.x + color.y * color.y + color.z * color.z);
    return vec4 (l, l, l, 1.0);
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

//
//float cubemap (vec3 cartesian) {
//    ivec3 fuv = cartesianToIndex (cartesian);
//    return grid [fuv.z * size * size + fuv.y * size + fuv.x];
//}

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

void main() {

    ivec2 pos = ivec2 (gl_GlobalInvocationID.yx);
    if (mode == MODE_GLOBE) {
        vec2 i = mapPos (pos);
        vec3 g = inverse (i);
        vec4 color = defaultColor;;
        // this provides some antialiasing at the rim of the globe by fading to dark blue over the outermost 1% of the radius
        vec3 c = toCartesian (g.xy);
        float pets = smoothstep (0.99, 1.00001, abs (g.z));
        ivec3 fuv = cartesianToIndex (c);
        int index = int (fuv.z * size * size + fuv.y * size + fuv.x);
        float v = (height_map_in [index] + 1.0) / 2.0;
        //v = float (fuv.y) / float (size);
        //color = findColor (v);
        //color = mix (color, vec4 (0.0, 0.0, 0.1, 1.0), pets);
        color = vec4 (v, v, v, 1.0);
        imageStore (destTex, pos, color);
    }

    if (mode == MODE_OVERVIEW) {
        vec2 i = mapPos (pos, true);
        vec3 g = vec3 (i.xy, 1.0);
        vec4 color = defaultColor;
        if (inBounds (g.xy)) {
            vec3 c = toCartesian (i.xy);
            ivec3 fuv = cartesianToIndex (c);
            int index = int (fuv.z * size * size + fuv.y * size + fuv.x);
            float v = height_map_in [index];
            color = findColor (v);

            vec3 f = forward (i);                                                   // get the geolocation of this texel in the inset map
            ivec2 s = scrPos (f.xy);                                                // find the corresponding texel in the main map
            if (f.z > 1.0 || f.z < 0.0 ||                                           // if the texel is out of the projection's  bounds or ...
            s.x < 0 || s.x > imageHeight * 2  || s.y < 0 || s.y > imageHeight) {    // if the texel is not on the main map ...
                color = toGreyscale (findColor (v));                                // ... grey out the corresponding texel in the inset map.
            }
        }
        imageStore (destTex, pos, color);
    }

}
