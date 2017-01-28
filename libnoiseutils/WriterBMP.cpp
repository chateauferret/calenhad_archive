#include "WriterBMP.h"
#include "Image.h"
#include "LittleEndian.h"
#include <fstream>

#include <noise/interp.h>
#include <noise/mathconsts.h>
#include <libnoise/exception.h>
#include <cstring>


/////////////////////////////////////////////////////////////////////////////
// WriterBMP class

using namespace noise::utils;

int WriterBMP::CalcWidthByteCount (int width) const
{
    return ((width * 3) + 3) & ~0x03;
}

void WriterBMP::WriteDestFile ()
{
    if (m_pSourceImage == NULL) {
        throw noise::ExceptionInvalidParam ();
    }

    int width  = m_pSourceImage->GetWidth  ();
    int height = m_pSourceImage->GetHeight ();

    // The width of one line in the file must be aligned on a 4-byte boundary.
    int bufferSize = CalcWidthByteCount (width);
    int destSize   = bufferSize * height;

    // This buffer holds one horizontal line in the destination file.
    noise::uint8* pLineBuffer = NULL;

    // File object used to write the file.
    std::ofstream os;
    os.clear ();

    // Allocate a buffer to hold one horizontal line in the bitmap.
    try {
        pLineBuffer = new noise::uint8[bufferSize];
    }
    catch (...) {
        throw noise::ExceptionOutOfMemory ();
    }

    // Open the destination file.
    os.open (m_destFilename.c_str (), std::ios::out | std::ios::binary);
    if (os.fail () || os.bad ()) {
        delete[] pLineBuffer;
        throw noise::ExceptionUnknown ();
    }

    // Build the header.
    noise::uint8 d[4];
    os.write ("BM", 2);
    os.write ((char*) LittleEndian::UnpackLittle32 (d, (uint32) destSize + NoiseConstants::BMP_HEADER_SIZE), 4);
    os.write ("\0\0\0\0", 4);
    os.write ((char*) LittleEndian::UnpackLittle32 (d, (uint32)  NoiseConstants::BMP_HEADER_SIZE), 4);
    os.write ((char*) LittleEndian::UnpackLittle32 (d, 40), 4);   // Palette offset
    os.write ((char*) LittleEndian::UnpackLittle32 (d, (uint32)  width ), 4);
    os.write ((char*) LittleEndian::UnpackLittle32 (d, (uint32) height), 4);
    os.write ((char*) LittleEndian::UnpackLittle16 (d, 1 ), 2);   // Planes per pixel
    os.write ((char*) LittleEndian::UnpackLittle16 (d, 24), 2);   // Bits per plane
    os.write ("\0\0\0\0", 4); // Compression (0 = none)
    os.write ((char*) LittleEndian::UnpackLittle32 (d, (uint32)destSize), 4);
    os.write ((char*) LittleEndian::UnpackLittle32 (d, 2834), 4); // X pixels per meter
    os.write ((char*) LittleEndian::UnpackLittle32 (d, 2834), 4); // Y pixels per meter
    os.write ("\0\0\0\0", 4);
    os.write ("\0\0\0\0", 4);
    if (os.fail () || os.bad ()) {
        os.clear ();
        os.close ();
        os.clear ();
        delete[] pLineBuffer;
        throw noise::ExceptionUnknown ();
    }

    // Build and write each horizontal line to the file.
    for (int y = 0; y < height; y++) {
        memset (pLineBuffer, 0, bufferSize);
        Color* pSource = m_pSourceImage->GetSlabPtr (y);
        noise::uint8* pDest   = pLineBuffer;
        for (int x = 0; x < width; x++) {
            *pDest++ = pSource->blue ;
            *pDest++ = pSource->green;
            *pDest++ = pSource->red  ;
            ++pSource;
        }
        os.write ((char*)pLineBuffer, (size_t)bufferSize);
        if (os.fail () || os.bad ()) {
            os.clear ();
            os.close ();
            os.clear ();
            delete[] pLineBuffer;
            throw noise::ExceptionUnknown ();
        }
    }

    os.close ();
    os.clear ();
    delete[] pLineBuffer;
}