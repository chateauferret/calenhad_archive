//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_LITTLEENDIAN_H
#define CALENHAD_LITTLEENDIAN_H

namespace noise {
    namespace utils {

        class LittleEndian {
        public:
            // Unpacks a floating-point value into four bytes.  This function is
            // specific to Intel machines.  A portable version will come soon (I
            // hope.)
            static inline uint8_t* UnpackFloat (uint8_t* bytes, float value) {
                uint8_t* pBytes = (uint8_t*) (&value);
                bytes[0] = *pBytes++;
                bytes[1] = *pBytes++;
                bytes[2] = *pBytes++;
                bytes[3] = *pBytes++;
                return bytes;
            }

            // Unpacks a 16-bit integer value into two bytes in little endian format.
            static inline uint8_t* UnpackLittle16 (uint8_t* bytes, uint16_t integer) {
                bytes[0] = (uint8_t) ((integer & 0x00ff));
                bytes[1] = (uint8_t) ((integer & 0xff00) >> 8);
                return bytes;
            }

            // Unpacks a 32-bit integer value into four bytes in little endian format.
            static inline uint8_t* UnpackLittle32 (uint8_t* bytes, uint32_t integer) {
                bytes[0] = (uint8_t) ((integer & 0x000000ff));
                bytes[1] = (uint8_t) ((integer & 0x0000ff00) >> 8);
                bytes[2] = (uint8_t) ((integer & 0x00ff0000) >> 16);
                bytes[3] = (uint8_t) ((integer & 0xff000000) >> 24);
                return bytes;
            }
        };
    }
}

#endif //CALENHAD_LITTLEENDIAN_H
