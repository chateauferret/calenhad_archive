//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_INTERPOLATION_H
#define CALENHAD_INTERPOLATION_H

#include <QColor>


namespace noise {
    namespace utils {
        class Interpolation {
        public:

            // Unpacks a 16-bit integer value into two bytes in little endian format.
            static inline uint8_t *UnpackLittle16(uint8_t *bytes, uint16_t integer) {
                bytes[0] = (uint8_t) ((integer & 0x00ff));
                bytes[1] = (uint8_t) ((integer & 0xff00) >> 8);
                return bytes;
            }

            // Unpacks a 32-bit integer value into four bytes in little endian format.
            static inline uint8_t *UnpackLittle32(uint8_t *bytes, uint32_t integer) {
                bytes[0] = (uint8_t) ((integer & 0x000000ff));
                bytes[1] = (uint8_t) ((integer & 0x0000ff00) >> 8);
                bytes[2] = (uint8_t) ((integer & 0x00ff0000) >> 16);
                bytes[3] = (uint8_t) ((integer & 0xff000000) >> 24);
                return bytes;
            }


            // Unpacks a floating-point value into four bytes.  This function is
            // specific to Intel machines.  A portable version will come soon (I
            // hope.)
            static inline uint8_t *UnpackFloat(uint8_t *bytes, float value) {
                uint8_t *pBytes = (uint8_t *) (&value);
                bytes[0] = *pBytes++;
                bytes[1] = *pBytes++;
                bytes[2] = *pBytes++;
                bytes[3] = *pBytes++;
                return bytes;
            }

            // Performs linear interpolation between two 8-bit channel values.
            static inline int blendChannel(const int channel0, const int channel1, float alpha) {
                float c0 = (float) (channel0 / 255.0);
                float c1 = (float) (channel1 / 255.0);
                return (int) (((c1 * alpha) + (c0 * (1.0f - alpha))) * 255.0f);
            }

            // Performs linear interpolation between two colors and stores the result
            // in out.
            static inline void linearInterpColor(const QColor &color0, const QColor &color1, float alpha, QColor &out) {
                out.setAlpha(blendChannel(color0.alpha(), color1.alpha(), alpha));
                out.setBlue(blendChannel(color0.blue(), color1.blue(), alpha));
                out.setGreen(blendChannel(color0.green(), color1.green(), alpha));
                out.setRed(blendChannel(color0.red(), color1.red(), alpha));
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
            static inline double cubicInterp(double n0, double n1, double n2, double n3, double a) {
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
            static inline double linearInterp(double n0, double n1, double a) {
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
            static inline double sCurve3(double a) {
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
            static inline double sCurve5(double a) {
                double a3 = a * a * a;
                double a4 = a3 * a;
                double a5 = a4 * a;
                return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
            }

            /// Clamps a value onto a clamping range.
            ///
            /// @param value The value to clamp.
            /// @param lowerBound The lower bound of the clamping range.
            /// @param upperBound The upper bound of the clamping range.
            ///
            /// @returns
            /// - @a value if @a value lies between @a lowerBound and @a upperBound.
            /// - @a lowerBound if @a value is less than @a lowerBound.
            /// - @a upperBound if @a value is greater than @a upperBound.

            static inline int ClampValue(int value, int lowerBound, int upperBound) {
                if (value < lowerBound) {
                    return lowerBound;
                } else if (value > upperBound) {
                    return upperBound;
                } else {
                    return value;
                }
            }


        };
    }
}





#endif //eCALENHAD_INTERPOLATION_H

