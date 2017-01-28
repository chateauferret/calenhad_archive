//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_WRITERTER_H
#define CALENHAD_WRITERTER_H

/// Terragen Terrain writer class.
///
/// This class creates a file in Terrage Terrain (*.ter) format given the
/// contents of a noise map object.  This class treats the values in the
/// noise map as elevations measured in meters.
///
/// <a href=http://www.planetside.co.uk/terragen/>Terragen</a> is a
/// terrain application that renders realistic landscapes.  Terragen is
/// available for Windows and MacOS; unfortunately, Terragen does not have
/// UN*X versions.
///
/// <b>Writing the noise map</b>
///
/// To write the noise map, perform the following steps:
/// - Pass the filename to the SetDestFilename() method.
/// - Pass a NoiseMap object to the SetSourceNoiseMap() method.
/// - Call the WriteDestFile() method.
///
/// The SetDestFilename() and SetSourceNoiseMap() methods must be called
/// before calling the WriteDestFile() method.

#include <string>
#include "NoiseContstants.h"

namespace noise {
    namespace utils {

        class NoiseMap;

        class WriterTER {

        public:

            /// Constructor.
            WriterTER () :
                    m_metersPerPoint ((float) NoiseConstants::DEFAULT_METERS_PER_POINT),
                    m_pSourceNoiseMap (NULL) {
            }

            /// Returns the name of the file to write.
            ///
            /// @returns The name of the file to write.
            std::string GetDestFilename () const {
                return m_destFilename;
            }

            /// Returns the distance separating adjacent points in the noise map,
            /// in meters.
            ///
            /// @returns The distance separating adjacent points in the noise map.
            float GetMetersPerPoint () const {
                return m_metersPerPoint;
            }

            /// Sets the name of the file to write.
            ///
            /// @param filename The name of the file to write.
            ///
            /// Call this method before calling the WriteDestFile() method.
            void SetDestFilename (const std::string& filename) {
                m_destFilename = filename;
            }

            /// Sets the distance separating adjacent points in the noise map, in
            /// meters.
            ///
            /// @param metersPerPoint The distance separating adjacent points in
            /// the noise map.
            void SetMetersPerPoint (float metersPerPoint) {
                m_metersPerPoint = metersPerPoint;
            }

            /// Sets the noise map object that is written to the file.
            ///
            /// @param sourceNoiseMap The noise map object to write.
            ///
            /// This object only stores a pointer to a noise map object, so make
            /// sure this object exists before calling the WriteDestFile() method.
            void SetSourceNoiseMap (NoiseMap& sourceNoiseMap) {
                m_pSourceNoiseMap = &sourceNoiseMap;
            }

            /// Writes the contents of the noise map object to the file.
            ///
            /// @pre SetDestFilename() has been previously called.
            /// @pre SetSourceNoiseMap() has been previously called.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            /// @throw noise::ExceptionUnknown An unknown exception occurred.
            /// Possibly the file could not be written.
            ///
            /// This method encodes the contents of the noise map and writes it to
            /// a file.  Before calling this method, call the SetSourceNoiseMap()
            /// method to specify the noise map, then call the SetDestFilename()
            /// method to specify the name of the file to write.
            ///
            /// This object assumes that the noise values represent elevations in
            /// meters.
            void WriteDestFile ();

        protected:

            /// Calculates the width of one horizontal line in the file, in bytes.
            ///
            /// @param width The width of the noise map, in points.
            ///
            /// @returns The width of one horizontal line in the file.
            int CalcWidthByteCount (int width) const;

            /// Name of the file to write.
            std::string m_destFilename;

            /// The distance separating adjacent points in the noise map, in
            /// meters.
            float m_metersPerPoint;

            /// A pointer to the noise map that will be written to the file.
            NoiseMap* m_pSourceNoiseMap;

        };
    }
}

#endif //CALENHAD_WRITERTER_H
