//
// Created by martin on 27/01/17.
//

#include "WriterTER.h"
#include "NoiseContstants.h"
#include "NoiseMap.h"
#include <fstream>
#include <libnoise/basictypes.h>
#include <libnoise/exception.h>
#include <cmath>
#include <libnoise/misc.h>
#include "LittleEndian.h"

using namespace noise::utils;
/////////////////////////////////////////////////////////////////////////////
// WriterTER class

int WriterTER::CalcWidthByteCount (int width) const
{
    return (width * sizeof (int16));
}

void WriterTER::WriteDestFile ()
{
    if (m_pSourceNoiseMap == NULL) {
        throw noise::ExceptionInvalidParam ();
    }

    int width  = m_pSourceNoiseMap->GetWidth  ();
    int height = m_pSourceNoiseMap->GetHeight ();

    int bufferSize = CalcWidthByteCount (width);
    int destSize   = bufferSize * height;

    // This buffer holds one horizontal line in the destination file.
    noise::uint8* pLineBuffer = NULL;

    // File object used to write the file.
    std::ofstream os;
    os.clear ();

    // Allocate a buffer to hold one horizontal line in the height map.
    try {
        pLineBuffer = new noise::uint8[bufferSize];
    }
    catch (...) {
        throw noise::ExceptionOutOfMemory ();
    }

    // Open the destination file.
    os.open (m_destFilename.c_str (), std::ios::out | std::ios::binary);
    if (os.fail () || os.bad ()) {
        os.clear ();
        delete[] pLineBuffer;
        throw noise::ExceptionUnknown ();
    }

    // Build the header.
    noise::uint8 d[4];
    int16 heightScale = (int16)(floor (32768.0 / (double)m_metersPerPoint));
    os.write ("TERRAGENTERRAIN ", 16);
    os.write ("SIZE", 4);
    os.write ((char*) LittleEndian::UnpackLittle16 (d, GetMin (width, height) - 1), 2);
    os.write ("\0\0", 2);
    os.write ("XPTS", 4);
    os.write ((char*) LittleEndian::UnpackLittle16 (d, width), 2);
    os.write ("\0\0", 2);
    os.write ("YPTS", 4);
    os.write ((char*) LittleEndian::UnpackLittle16 (d, height), 2);
    os.write ("\0\0", 2);
    os.write ("SCAL", 4);
    os.write ((char*) LittleEndian::UnpackFloat (d, m_metersPerPoint), 4);
    os.write ((char*) LittleEndian::UnpackFloat (d, m_metersPerPoint), 4);
    os.write ((char*) LittleEndian::UnpackFloat (d, m_metersPerPoint), 4);
    os.write ("ALTW", 4);
    os.write ((char*) LittleEndian::UnpackLittle16 (d, heightScale), 2);
    os.write ("\0\0", 2);
    if (os.fail () || os.bad ()) {
        os.clear ();
        os.close ();
        os.clear ();
        delete[] pLineBuffer;
        throw noise::ExceptionUnknown ();
    }

    // Build and write each horizontal line to the file.
    for (int y = 0; y < height; y++) {
        float* pSource = m_pSourceNoiseMap->GetSlabPtr (y);
        noise::uint8* pDest   = pLineBuffer;
        for (int x = 0; x < width; x++) {
            int16 scaledHeight = (int16)(floor (*pSource * 2.0));
            LittleEndian::UnpackLittle16 (pDest, scaledHeight);
            pDest += 2;
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
