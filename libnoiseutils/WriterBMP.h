//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_WRITERBMP_H
#define CALENHAD_WRITERBMP_H


/// Windows bitmap image writer class.
///
/// This class creates a file in Windows bitmap (*.bmp) format given the
/// contents of an image object.
///
/// <b>Writing the image</b>
///
/// To write the image to a file, perform the following steps:
/// - Pass the filename to the SetDestFilename() method.
/// - Pass an Image object to the SetSourceImage() method.
/// - Call the WriteDestFile() method.
///
/// The SetDestFilename() and SetSourceImage() methods must be called
/// before calling the WriteDestFile() method.
#include <string>
#include <cstdlib>
#include "NoiseContstants.h"

namespace noise {
    namespace utils {

        class Image;

        class WriterBMP {

        public:

            /// Constructor.
            WriterBMP () :
                    m_pSourceImage (NULL) {
            }

            /// Returns the name of the file to write.
            ///
            /// @returns The name of the file to write.
            std::string GetDestFilename () const {
                return m_destFilename;
            }

            /// Sets the name of the file to write.
            ///
            /// @param filename The name of the file to write.
            ///
            /// Call this method before calling the WriteDestFile() method.
            void SetDestFilename (const std::string& filename) {
                m_destFilename = filename;
            }

            /// Sets the image object that is written to the file.
            ///
            /// @param sourceImage The image object to write.
            ///
            /// This object only stores a pointer to an image object, so make sure
            /// this object exists before calling the WriteDestFile() method.
            void SetSourceImage (Image& sourceImage) {
                m_pSourceImage = &sourceImage;
            }

            /// Writes the contents of the image object to the file.
            ///
            /// @pre SetDestFilename() has been previously called.
            /// @pre SetSourceImage() has been previously called.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            /// @throw noise::ExceptionUnknown An unknown exception occurred.
            /// Possibly the file could not be written.
            ///
            /// This method encodes the contents of the image and writes it to a
            /// file.  Before calling this method, call the SetSourceImage()
            /// method to specify the image, then call the SetDestFilename()
            /// method to specify the name of the file to write.
            void WriteDestFile ();

        protected:

            /// Calculates the width of one horizontal line in the file, in bytes.
            ///
            /// @param width The width of the image, in points.
            ///
            /// @returns The width of one horizontal line in the file.
            ///
            /// Windows bitmap files require that the width of one horizontal line
            /// must be aligned to a 32-bit boundary.
            int CalcWidthByteCount (int width) const;

            /// Name of the file to write.
            std::string m_destFilename;

            /// A pointer to the image object that will be written to the file.
            Image* m_pSourceImage;

        };
    }
}
#endif //CALENHAD_WRITERBMP_H
