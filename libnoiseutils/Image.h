//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_IMAGE_H
#define CALENHAD_IMAGE_H

#include <cstdlib>
#include "NoiseContstants.h"
#include "Color.h"

/// Implements an image, a 2-dimensional array of color values.
///
/// An image can be used to store a color texture.
///
/// These color values are of type Color.
///
/// The size (width and height) of the image can be specified during
/// object construction or at any other time.
///
/// The GetValue() and SetValue() methods can be used to access individual
/// color values stored in the image.
///
/// This class manages its own memory.  If you copy an image object into
/// another image object, the original contents of the image object will
/// be freed.
///
/// If you specify a new size for the image and the new size is smaller
/// than the current size, the allocated memory will not be reallocated.
/// Call ReclaimMem() to reclaim the wasted memory.
///
/// <b>Border Values</b>
///
/// All of the color values outside of the image are assumed to have a
/// common color value known as the <i>border value</i>.
///
/// To set the border value, call the SetBorderValue() method.
///
/// The GetValue() method returns the border value if the specified
/// position lies outside of the image.
///
/// <b>Internal Image Structure</b>
///
/// Internally, the color values are organized into horizontal rows called
/// @a slabs.  Slabs are ordered from bottom to top.
///
/// Each slab contains a contiguous row of color values in memory.  The
/// color values in a slab are organized left to right.  These values are
/// of type Color.
///
/// The offset between the starting points of any two adjacent slabs is
/// called the <i>stride amount</i>.  The stride amount is measured by the
/// number of Color objects between these two starting points, not by the
/// number of bytes.  For efficiency reasons, the stride is often a
/// multiple of the machine word size.
///
/// The GetSlabPtr() methods allow you to retrieve pointers to the slabs
/// themselves.

namespace noise {
    namespace utils {


        class Image {

        public:

            /// Constructor.
            ///
            /// Creates an empty image.
            Image ();

            /// Constructor.
            ///
            /// @param width The width of the new image.
            /// @param height The height of the new image.
            ///
            /// @pre The width and height values are positive.
            /// @pre The width and height values do not exceed the maximum
            /// possible width and height for the image.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            ///
            /// Creates an image with uninitialized color values.
            ///
            /// It is considered an error if the specified dimensions are not
            /// positive.
            Image (int width, int height);

            /// Copy constructor.
            ///
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            Image (const Image& rhs);

            /// Destructor.
            ///
            /// Frees the allocated memory for the image.
            ~Image ();

            /// Assignment operator.
            ///
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            ///
            /// @returns Reference to self.
            ///
            /// Creates a copy of the image.
            Image& operator= (const Image& rhs);

            /// Clears the image to a specified color value.
            ///
            /// @param value The color value that all positions within the image
            /// are cleared to.
            void Clear (const Color& value);

            /// Returns the color value used for all positions outside of the
            /// image.
            ///
            /// @returns The color value used for all positions outside of the
            /// image.
            ///
            /// All positions outside of the image are assumed to have a common
            /// color value known as the <i>border value</i>.
            Color GetBorderValue () const {
                return m_borderValue;
            }

            /// Returns a const pointer to a slab.
            ///
            /// @returns A const pointer to a slab at the position (0, 0), or
            /// @a NULL if the image is empty.
            const Color* GetConstSlabPtr () const {
                return m_pImage;
            }

            /// Returns a const pointer to a slab at the specified row.
            ///
            /// @param row The row, or @a y coordinate.
            ///
            /// @returns A const pointer to a slab at the position ( 0, @a row ),
            /// or @a NULL if the image is empty.
            ///
            /// @pre The coordinates must exist within the bounds of the image.
            ///
            /// This method does not perform bounds checking so be careful when
            /// calling it.
            const Color* GetConstSlabPtr (int row) const {
                return GetConstSlabPtr (0, row);
            }

            /// Returns a const pointer to a slab at the specified position.
            ///
            /// @param x The x coordinate of the position.
            /// @param y The y coordinate of the position.
            ///
            /// @returns A const pointer to a slab at the position ( @a x, @a y ),
            /// or @a NULL if the image is empty.
            ///
            /// @pre The coordinates must exist within the bounds of the image.
            ///
            /// This method does not perform bounds checking so be careful when
            /// calling it.
            const Color* GetConstSlabPtr (int x, int y) const {
                return m_pImage + (size_t) x + (size_t) m_stride * (size_t) y;
            }

            /// Returns the height of the image.
            ///
            /// @returns The height of the image.
            int GetHeight () const {
                return m_height;
            }

            /// Returns the amount of memory allocated for this image.
            ///
            /// @returns The amount of memory allocated for this image.
            ///
            /// This method returns the number of Color objects allocated.
            size_t GetMemUsed () const {
                return m_memUsed;
            }

            /// Returns a pointer to a slab.
            ///
            /// @returns A pointer to a slab at the position (0, 0), or @a NULL if
            /// the image is empty.
            Color* GetSlabPtr () {
                return m_pImage;
            }

            /// Returns a pointer to a slab at the specified row.
            ///
            /// @param row The row, or @a y coordinate.
            ///
            /// @returns A pointer to a slab at the position ( 0, @a row ), or
            /// @a NULL if the image is empty.
            ///
            /// @pre The coordinates must exist within the bounds of the image.
            ///
            /// This method does not perform bounds checking so be careful when
            /// calling it.
            Color* GetSlabPtr (int row) {
                return GetSlabPtr (0, row);
            }

            /// Returns a pointer to a slab at the specified position.
            ///
            /// @param x The x coordinate of the position.
            /// @param y The y coordinate of the position.
            ///
            /// @returns A pointer to a slab at the position ( @a x, @a y ), or
            /// @a NULL if the image is empty.
            ///
            /// @pre The coordinates must exist within the bounds of the image.
            ///
            /// This method does not perform bounds checking so be careful when
            /// calling it.
            Color* GetSlabPtr (int x, int y) {
                return m_pImage + (size_t) x + (size_t) m_stride * (size_t) y;
            }

            /// Returns the stride amount of the image.
            ///
            /// @returns The stride amount of the image.
            ///
            /// - The <i>stride amount</i> is the offset between the starting
            ///   points of any two adjacent slabs in an image.
            /// - The stride amount is measured by the number of Color objects
            ///   between these two points, not by the number of bytes.
            int GetStride () const {
                return m_stride;
            }

            /// Returns a color value from the specified position in the image.
            ///
            /// @param x The x coordinate of the position.
            /// @param y The y coordinate of the position.
            ///
            /// @returns The color value at that position.
            ///
            /// This method returns the border value if the coordinates exist
            /// outside of the image.
            Color GetValue (int x, int y) const;

            /// Returns the width of the image.
            ///
            /// @returns The width of the image.
            int GetWidth () const {
                return m_width;
            }

            /// Reallocates the image to recover wasted memory.
            ///
            /// @throw noise::ExceptionOutOfMemory Out of memory.  (Yes, this
            /// method can return an out-of-memory exception because two images
            /// will exist temporarily in memory during this call.)
            ///
            /// The contents of the image is unaffected.
            void ReclaimMem ();

            /// Sets the color value to use for all positions outside of the
            /// image.
            ///
            /// @param borderValue The color value to use for all positions
            /// outside of the image.
            ///
            /// All positions outside of the image are assumed to have a common
            /// color value known as the <i>border value</i>.
            void SetBorderValue (const Color& borderValue) {
                m_borderValue = borderValue;
            }

            /// Sets the new size for the image.
            ///
            /// @param width The new width for the image.
            /// @param height The new height for the image.
            ///
            /// @pre The width and height values are positive.
            /// @pre The width and height values do not exceed the maximum
            /// possible width and height for the image.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            ///
            /// On exit, the contents of the image are undefined.
            ///
            /// If the @a OUT_OF_MEMORY exception occurs, this image becomes
            /// empty.
            ///
            /// If the @a INVALID_PARAM exception occurs, the image is unmodified.
            void SetSize (int width, int height);

            /// Sets a color value at a specified position in the image.
            ///
            /// @param x The x coordinate of the position.
            /// @param y The y coordinate of the position.
            /// @param value The color value to set at the given position.
            ///
            /// This method does nothing if the image is empty or the position is
            /// outside the bounds of the image.
            void SetValue (int x, int y, const Color& value);

            /// Takes ownership of the buffer within the source image.
            ///
            /// @param source The source image.
            ///
            /// On exit, the source image object becomes empty.
            ///
            /// This method only moves the buffer pointer so this method is very
            /// quick.
            void TakeOwnership (Image& source);

        private:

            /// Returns the minimum amount of memory required to store an image of
            /// the specified size.
            ///
            /// @param width The width of the image.
            /// @param height The height of the image.
            ///
            /// @returns The minimum amount of memory required to store the image.
            ///
            /// The returned color value is measured by the number of Color
            /// objects required to store the image, not by the number of bytes.
            size_t CalcMinMemUsage (int width, int height) const {
                return CalcStride ((size_t) width) * (size_t) height;
            }

            /// Calculates the stride amount for an image.
            ///
            /// @param width The width of the image.
            ///
            /// @returns The stride amount.
            ///
            /// - The <i>stride amount</i> is the offset between the starting
            ///   points of any two adjacent slabs in an image.
            /// - The stride amount is measured by the number of Color objects
            ///   between these two points, not by the number of bytes.
            size_t CalcStride (int width) const {
                return (size_t) (((width + NoiseConstants::RASTER_STRIDE_BOUNDARY - 1)
                                  / NoiseConstants::RASTER_STRIDE_BOUNDARY) * NoiseConstants::RASTER_STRIDE_BOUNDARY);
            }

            /// Copies the contents of the buffer in the source image into this
            /// image.
            ///
            /// @param source The source image.
            ///
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            ///
            /// This method reallocates the buffer in this image object if
            /// necessary.
            ///
            /// @warning This method calls the standard library function
            /// @a memcpy, which probably violates the DMCA because it can be used
            /// to make a bitwise copy of anything, like, say, a DVD.  Don't call
            /// this method if you live in the USA.
            void CopyImage (const Image& source);

            /// Resets the image object.
            ///
            /// This method is similar to the InitObj() method, except this method
            /// deletes the memory allocated to the image.
            void DeleteImageAndReset ();

            /// Initializes the image object.
            ///
            /// @pre Must be called during object construction.
            /// @pre The image buffer must not exist.
            void InitObj ();

            /// The Color value used for all positions outside of the image.
            Color m_borderValue;

            /// The current height of the image.
            int m_height;

            /// The amount of memory allocated for the image.
            ///
            /// This value is equal to the number of Color objects allocated for
            /// the image, not the number of bytes.
            size_t m_memUsed;

            /// A pointer to the image buffer.
            Color* m_pImage;

            /// The stride amount of the image.
            int m_stride;

            /// The current width of the image.
            int m_width;

        };

    }
}
#endif //CALENHAD_IMAGE_H
