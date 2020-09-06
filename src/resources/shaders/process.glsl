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



void main() {

}
