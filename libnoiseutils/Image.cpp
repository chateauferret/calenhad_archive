//
// Created by martin on 27/01/17.
//

#include <cstring>
#include <libnoise/exception.h>
#include "Image.h"
using namespace noise::utils;

//////////////////////////////////////////////////////////////////////////////
// Image class

Image::Image ()
{
    InitObj ();
}

Image::Image (int width, int height)
{
    InitObj ();
    SetSize (width, height);
}

Image::Image (const Image& rhs)
{
    InitObj ();
    CopyImage (rhs);
}

Image::~Image ()
{
    delete[] m_pImage;
}

Image& Image::operator= (const Image& rhs)
{
    CopyImage (rhs);

    return *this;
}

void Image::Clear (const Color& value)
{
    if (m_pImage != NULL) {
        for (int y = 0; y < m_height; y++) {
            Color* pDest = GetSlabPtr (0, y);
            for (int x = 0; x < m_width; x++) {
                *pDest++ = value;
            }
        }
    }
}

void Image::CopyImage (const Image& source)
{
    // Resize the image buffer, then copy the slabs from the source image
    // buffer to this image buffer.
    SetSize (source.GetWidth (), source.GetHeight ());
    for (int y = 0; y < source.GetHeight (); y++) {
        const Color* pSource = source.GetConstSlabPtr (0, y);
        Color* pDest = GetSlabPtr (0, y);
        memcpy (pDest, pSource, (size_t)source.GetWidth () * sizeof (float));
    }

    // Copy the border value as well.
    m_borderValue = source.m_borderValue;
}

void Image::DeleteImageAndReset ()
{
    delete[] m_pImage;
    InitObj ();
}

Color Image::GetValue (int x, int y) const
{
    if (m_pImage != NULL) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            return *(GetConstSlabPtr (x, y));
        }
    }
    // The coordinates specified are outside the image.  Return the border
    // value.
    return m_borderValue;
}

void Image::InitObj ()
{
    m_pImage  = NULL;
    m_height  = 0;
    m_width   = 0;
    m_stride  = 0;
    m_memUsed = 0;
    m_borderValue = Color (0, 0, 0, 0);
}

void Image::ReclaimMem ()
{
    size_t newMemUsage = CalcMinMemUsage (m_width, m_height);
    if (m_memUsed > newMemUsage) {
        // There is wasted memory.  Create the smallest buffer that can fit the
        // data and copy the data to it.
        Color* pNewImage = NULL;
        try {
            pNewImage = new Color[newMemUsage];
        }
        catch (...) {
            throw noise::ExceptionOutOfMemory ();
        }
        memcpy (pNewImage, m_pImage, newMemUsage * sizeof (float));
        delete[] m_pImage;
        m_pImage = pNewImage;
        m_memUsed = newMemUsage;
    }
}

void Image::SetSize (int width, int height)
{
    if (width < 0 || height < 0
        || width > NoiseConstants::RASTER_MAX_WIDTH || height > NoiseConstants::RASTER_MAX_HEIGHT) {
        // Invalid width or height.
        throw noise::ExceptionInvalidParam ();
    } else if (width == 0 || height == 0) {
        // An empty image was specified.  Delete it and zero out the size member
        // variables.
        DeleteImageAndReset ();
    } else {
        // A new image size was specified.  Allocate a new image buffer unless
        // the current buffer is large enough for the new image (we don't want
        // costly reallocations going on.)
        size_t newMemUsage = CalcMinMemUsage (width, height);
        if (m_memUsed < newMemUsage) {
            // The new size is too big for the current image buffer.  We need to
            // reallocate.
            DeleteImageAndReset ();
            try {
                m_pImage = new Color[newMemUsage];
            }
            catch (...) {
                throw noise::ExceptionOutOfMemory ();
            }
            m_memUsed = newMemUsage;
        }
        m_stride = (int)CalcStride (width);
        m_width  = width;
        m_height = height;
    }
}

void Image::SetValue (int x, int y, const Color& value)
{
    if (m_pImage != NULL) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            *(GetSlabPtr (x, y)) = value;
        }
    }
}

void Image::TakeOwnership (Image& source)
{
    // Copy the values and the image buffer from the source image to this image.
    // Now this image pwnz the source buffer.
    delete[] m_pImage;
    m_memUsed = source.m_memUsed;
    m_height  = source.m_height;
    m_pImage  = source.m_pImage;
    m_stride  = source.m_stride;
    m_width   = source.m_width;

    // Now that the source buffer is assigned to this image, reset the source
    // image object.
    source.InitObj ();
}