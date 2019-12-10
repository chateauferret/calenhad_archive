//
// Created by martin on 02/01/18.
//

#ifndef CALENHAD_NOISECONSTANTS_H
#define CALENHAD_NOISECONSTANTS_H

namespace calenhad {
    namespace noiseconstants {


        /// Generates coherent noise quickly.  When a coherent-noise function with
        /// this quality setting is used to generate a bump-map image, there are
        /// noticeable "creasing" artifacts in the resulting image.  This is
        /// because the derivative of that function is discontinuous at integer
        /// boundaries.
        static constexpr int QUALITY_FAST = 0;

        /// Generates standard-quality coherent noise.  When a coherent-noise
        /// function with this quality setting is used to generate a bump-map
        /// image, there are some minor "creasing" artifacts in the resulting
        /// image.  This is because the second derivative of that function is
        /// discontinuous at integer boundaries.
        static constexpr int QUALITY_STD = 1;

        /// Generates the best-quality coherent noise.  When a coherent-noise
        /// function with this quality setting is used to generate a bump-map
        /// image, there are no "creasing" artifacts in the resulting image.  This
        /// is because the first and second derivatives of that function are
        /// continuous at integer boundaries.
        static constexpr int QUALITY_BEST = 2;

        static constexpr int DEFAULT_QUALITY = QUALITY_STD;
        static constexpr int DEFAULT_PERLIN_SEED = 0;
        static constexpr int MAX_OCTAVES = 30;

        static constexpr double DEFAULT_CYLINDERS_FREQUENCY = 1.0;
        static constexpr double DEFAULT_SPHERES_FREQUENCY = 1.0;
        static constexpr double DEFAULT_EXPONENT = 2.0;
        static constexpr double DEFAULT_TRANSLATE_POINT_X = 1.0;
        static constexpr double DEFAULT_TRANSLATE_POINT_Y = 1.0;
        static constexpr double DEFAULT_TRANSLATE_POINT_Z = 1.0;
        static constexpr double DEFAULT_ROTATE_X = 0.0;
        static constexpr double DEFAULT_ROTATE_Y = 0.0;
        static constexpr double DEFAULT_ROTATE_Z = 0.0;
        static constexpr double DEFAULT_CLAMP_LOWER_BOUND = -1.0;
        static constexpr double DEFAULT_CLAMP_UPPER_BOUND = 1.0;
        static constexpr double DEFAULT_CONST_VALUE = 0.0;
        static constexpr double DEFAULT_SCALE = 1.0;
        static constexpr double DEFAULT_BIAS = 0.0;
        static constexpr double DEFAULT_SELECT_LOWER_BOUND = -1.0;
        static constexpr double DEFAULT_SELECT_UPPER_BOUND = 1.0;
        static constexpr double DEFAULT_SELECT_EDGE_FALLOFF = 0.1;
        static constexpr double DEFAULT_SCALE_POINT_X = 1.0;
        static constexpr double DEFAULT_SCALE_POINT_Y = 1.0;
        static constexpr double DEFAULT_SCALE_POINT_Z = 1.0;

        static constexpr double DEFAULT_PERLIN_FREQUENCY = 1.0;
        static constexpr double DEFAULT_PERLIN_LACUNARITY = 2.0;
        static constexpr int DEFAULT_PERLIN_OCTAVE_COUNT = 6;
        static constexpr double DEFAULT_PERLIN_PERSISTENCE = 0.5;

        static constexpr double DEFAULT_BILLOW_FREQUENCY = 1.0;
        static constexpr double DEFAULT_BILLOW_LACUNARITY = 2.0;
        static constexpr int DEFAULT_BILLOW_OCTAVE_COUNT = 6;
        static constexpr double DEFAULT_BILLOW_PERSISTENCE = 0.5;

        static constexpr double DEFAULT_RIDGED_FREQUENCY = 1.0;
        static constexpr double DEFAULT_RIDGED_LACUNARITY = 2.0;
        static constexpr int DEFAULT_RIDGED_OCTAVE_COUNT = 6;

        static constexpr double DEFAULT_VORONOI_FREQUENCY = 1.0;
        static constexpr double DEFAULT_VORONOI_DISPLACEMENT = 2.0;
        static constexpr int DEFAULT_VORONOI_OCTAVE_COUNT = 6;
        static constexpr double DEFAULT_VORONOI_PERSISTENCE = 0.5;

        static constexpr double DEFAULT_TURBULENCE_FREQUENCY = 1.0;
        static constexpr double DEFAULT_TURBULENCE_POWER = 2.0;
        static constexpr double DEFAULT_TURBULENCE_ROUGHNESS = 1.0;

    };
}
#endif //CALENHAD_NOISECONSTANTS_H
