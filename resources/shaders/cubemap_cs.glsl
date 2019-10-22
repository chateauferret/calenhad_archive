#version 430
layout(local_size_x = 32, local_size_y = 32) in;
layout (rgba32f, binding = 0) uniform image2D destTex;          // output texture
layout (binding = 1) uniform sampler2DArray rasters;            // array of input textures for modules that require them
layout (std430, binding = 2) buffer colorMapBuffer { vec4 color_map_in []; };
layout (std430, binding = 3) buffer heightMapBuffer { float height_map_out []; };
//layout (std430, binding = 4) buffer vertexBuffer { vec4 vertices []; };
layout (std430, binding = 4) buffer gridBuffer { float points []; };

uniform int pointCount;
uniform int colorMapBufferSize;
uniform int imageHeight = 512;
uniform int insetHeight = 64;

// first element is the datum longitude, second element is the datum latitude, third element is the scale
uniform vec3 datum;

// mathematical constants
#define M_PI 3.1415926535898
#define M_PI_2 1.57079632679
#define SQRT_3 1.732050808


// projection types
const int PROJ_EQUIRECTANGULAR = 0;
const int PROJ_MERCATOR = 1;
const int PROJ_ORTHOGRAPHIC = 2;
//...

// cube faces
const int FACE_FRONT = 0, FACE_BACK = 3, FACE_NORTH = 1, FACE_SOUTH = 4, FACE_WEST = 2, FACE_EAST = 5;

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

// raster buffer parameters
uniform int rasterResolution;                                      // number of elements in a raster = resolution * resolution * 2

vec4 toCartesian (in vec3 geolocation) { // x = longitude, y = latitude
    vec4 cartesian;
    cartesian.x = cos (geolocation.x) * cos (geolocation.y);
    cartesian.z = cos (geolocation.y) * sin (geolocation.x);
    cartesian.y = sin (geolocation.y);

    cartesian.w = geolocation.z;
    return cartesian;
}

vec2 toGeolocation (vec3 cartesian) {
    float phi = atan (cartesian.z, cartesian.x);
    float theta = acos (cartesian.y) - (M_PI / 2);
    return vec2 (phi, -theta);
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

// Find the colour in the current legend corresponding to the given noise value. This works the same way as map, above.
vec4 findColor (float value) {
    value = value / 2 + 0.5;
    float index = value * colorMapBufferSize;
    int indexPos = int (index);
    int index0 = clamp (indexPos    , 0, colorMapBufferSize - 1);
    int index1 = clamp (indexPos + 1, 0, colorMapBufferSize - 1);
    ivec2 pos = ivec2 (gl_GlobalInvocationID.xy);
    vec4 out0 = color_map_in [index0 / 4];
    vec4 out1 = color_map_in [index1 / 4];
    float alpha = (index - index0) / (index1 - index0);
    return mix (vec4 (out0.xyz, 1.0), vec4 (out1.xyz, 1.0), alpha);
}

// inserted code //

// Coordinate systems:
//      screen coordinates (corresponds to GlobalInvoicationID) - the coordinates of the texel being rendered. 0 <= x <= resolution * 2; 0 <= x <= resolution.
//      map coordinates - the coordinates of a point on the logical map. -2 * M_PI < x <= 2 * M_PI; -M_PI <= y <= M_PI.
//          (the logical map is the equirectangular projection with scale 1 unit = 1 radian of latitude or longitude).
//      geolocation - the latitude and longitude (in that order) of a point on the geoid under the reigning projection. -2 * M_PI < x <= 2 * M_PI; -M_PI <= y <= M_PI.
//      cartesian - the point in 3D space corresponding to a geolocation on the surface of the (spherical) geoid. -1 <= x, y, z <= 1 and x^2 + y^2 + z^2 = 1.

// Returns the map coordinates for a given GlobalInvocationID (essentially screen coordinates) taking into account the scale.
vec2 mapPos (vec2 pos, bool inset) {
    float h = (inset ? insetHeight : imageHeight);
    vec2 j = vec2 ((pos.x - h) / (h * 2), (pos.y / 2 - (h / 4)) / h);
    j *= M_PI * 2;
    if (! inset) { j *= datum.z; }
    return j;
}

vec2 mapPos (vec2 pos) {
    return mapPos (pos, false);
}

// Returns the screen coordinates (GlobalInvocationID) for given map coordinates taking into account the scale.

ivec2 scrPos (vec2 g, bool inset) {
    vec2 j = g / (inset ? 1.0 : datum.z);
    j /= M_PI;
    j.x = (j.x + 1) * (inset ? insetHeight : imageHeight);
    j.y = (j.y + 0.5) * (inset ? insetHeight : imageHeight);
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


    // inserted forward //

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

    // inserted inverse //

    return vec3 (0.0, 0.0, -1.0);

}

vec4 toGreyscale (vec4 color) {
    float l = sqrt (color.x * color.x + color.y * color.y + color.z * color.z);
    return vec4 (l, l, l, 1.0);
}

vec3 indexToCartesian (int face, vec2 uv) {
    vec3 xyz;           // cartesian coordinates on the sphere
    float x, y, z;      // cartesian coordinates on the cube
    if (face == FACE_NORTH)  { y =  1.0; x = uv.x; y = uv.y; }
    if (face == FACE_SOUTH)  { y = -1.0; x = uv.x; y = uv.y; }
    if (face == FACE_EAST)   { x =  1.0; z = uv.x; y = uv.y; }
    if (face == FACE_WEST)   { x = -1.0; z = uv.x; y = uv.y; }
    if (face == FACE_FRONT)  { z =  1.0; x = uv.x; z = uv.y; }
    if (face == FACE_BACK)   { z = -1.0; x = uv.x; z = uv.y; }
    xyz.x = x * sqrt (1.0f - y * y * 0.5f - z * z * 0.5f + y * y * z * z / 3.0f);
    xyz.y = y * sqrt (1.0f - z * z * 0.5f - x * x * 0.5f + z * z * x * x / 3.0f);
    xyz.z = z * sqrt (1.0f - x * x * 0.5f - y * y * 0.5f + x * x * y * y / 3.0f);
    return xyz;
}

void main() {

    if (mode == MODE_GLOBE) {

        ivec2 pos = ivec2 (gl_GlobalInvocationID.yx);
        vec2 i = mapPos (pos);
        vec3 g = inverse (i);
        vec4 c = toCartesian (g);
        vec4 color;

        // this provides some antialiasing at the rim of the globe by fading to dark blue over the outermost 1% of the radius
        float pets = smoothstep (0.99, 1.00001, abs (c.w));
        int index = cartesianToIndex (c);
        float v = grid [index];
        color = findColor (v);
        color = mix (color, vec4 (0.0, 0.0, 0.1, 1.0), pets);

        imageStore (destTex, pos, color);
        height_map_out [pos.y * imageHeight * 2 + pos.x] = v;
    }

    if (mode == MODE_OVERVIEW || mode == MODE_PREVIEW) {
        ivec2 pos = ivec2 (gl_GlobalInvocationID.yx);
        vec2 i = mapPos (pos, mode == MODE_OVERVIEW);
        vec3 g = vec3 (i.xy, 1.0); //nverse (i);
        vec4 c = toCartesian (g);
        vec4 color;
        int index = cartesianToIndex (c);
        float v = grid [index];
        color = findColor (v);

        if (mode == MODE_OVERVIEW) {
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
