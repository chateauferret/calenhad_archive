//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_INTERPOLATION_H
#define CALENHAD_INTERPOLATION_H

#include <libnoise/basictypes.h>
#include "Color.h"
#include "LittleEndian.h"
#include <noise/interp.h>


namespace noise {
    namespace utils {
        class Interpolation {
        public:
            // Performs linear interpolation between two 8-bit channel values.
            static inline noise::uint8 BlendChannel (const uint8 channel0, const uint8 channel1, float alpha) {
                float c0 = (float) channel0 / 255.0;
                float c1 = (float) channel1 / 255.0;
                return (noise::uint8) (((c1 * alpha) + (c0 * (1.0f - alpha))) * 255.0f);
            }

            // Performs linear interpolation between two colors and stores the result
            // in out.
            static inline void LinearInterpColor (const Color& color0, const Color& color1, float alpha, Color& out) {
                out.alpha = BlendChannel (color0.alpha, color1.alpha, alpha);
                out.blue = BlendChannel (color0.blue, color1.blue, alpha);
                out.green = BlendChannel (color0.green, color1.green, alpha);
                out.red = BlendChannel (color0.red, color1.red, alpha);
            }


            /// Performs cubic interpolation between two values bound between two other
            /// values.
            ///
            /// @param n0 The value before the first value.
            /// @param n1 The first value.
            /// @param n2 The second value.
            /// @param n3 The value after the second value.
            /// @param a The alpha value.
            ///
            /// @returns The interpolated value.
            ///
            /// The alpha value should range from 0.0 to 1.0.  If the alpha value is
            /// 0.0, this function returns @a n1.  If the alpha value is 1.0, this
            /// function returns @a n2.
            static inline double CubicInterp (double n0, double n1, double n2, double n3, double a) {
                double p = (n3 - n2) - (n0 - n1);
                double q = (n0 - n1) - p;
                double r = n2 - n0;
                double s = n1;
                return p * a * a * a + q * a * a + r * a + s;
            }

            /// Performs linear interpolation between two values.
            ///
            /// @param n0 The first value.
            /// @param n1 The second value.
            /// @param a The alpha value.
            ///
            /// @returns The interpolated value.
            ///
            /// The alpha value should range from 0.0 to 1.0.  If the alpha value is
            /// 0.0, this function returns @a n0.  If the alpha value is 1.0, this
            /// function returns @a n1.
            static inline double LinearInterp (double n0, double n1, double a) {
                return ((1.0 - a) * n0) + (a * n1);
            }

            /// Maps a value onto a cubic S-curve.
            ///
            /// @param a The value to map onto a cubic S-curve.
            ///
            /// @returns The mapped value.
            ///
            /// @a a should range from 0.0 to 1.0.
            ///
            /// The derivative of a cubic S-curve is zero at @a a = 0.0 and @a a =
            /// 1.0
            static inline double SCurve3 (double a) {
                return (a * a * (3.0 - 2.0 * a));
            }

            /// Maps a value onto a quintic S-curve.
            ///
            /// @param a The value to map onto a quintic S-curve.
            ///
            /// @returns The mapped value.
            ///
            /// @a a should range from 0.0 to 1.0.
            ///
            /// The first derivative of a quintic S-curve is zero at @a a = 0.0 and
            /// @a a = 1.0
            ///
            /// The second derivative of a quintic S-curve is zero at @a a = 0.0 and
            /// @a a = 1.0
            static inline double SCurve5 (double a) {
                double a3 = a * a * a;
                double a4 = a3 * a;
                double a5 = a4 * a;
                return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
            }

        };

    }

}

#endif //eCALENHAD_INTERPOLATION_H

