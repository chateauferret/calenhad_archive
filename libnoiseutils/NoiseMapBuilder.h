//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_NOISEMAPBUILDER_H
#define CALENHAD_NOISEMAPBUILDER_H



/// Abstract base class for a noise-map builder
///
/// A builder class builds a noise map by filling it with coherent-noise
/// values generated from the surface of a three-dimensional mathematical
/// object.  Each builder class defines a specific three-dimensional
/// surface, such as a cylinder, sphere, or plane.
///
/// A builder class describes these input values using a coordinate system
/// applicable for the mathematical object (e.g., a latitude/longitude
/// coordinate system for the spherical noise-map builder.)  It then
/// "flattens" these coordinates onto a plane so that it can write the
/// coherent-noise values into a two-dimensional noise map.
///
/// <b>Building the Noise Map</b>
///
/// To build the noise map, perform the following steps:
/// - Pass the bounding coordinates to the SetBounds() method.
/// - Pass the noise map size, in points, to the SetDestSize() method.
/// - Pass a NoiseMap object to the SetDestNoiseMap() method.
/// - Pass a noise module (derived from noise::module::Module) to the
///   SetSourceModule() method.
/// - Call the Build() method.
///
/// You may also pass a callback function to the SetCallback() method.
/// The Build() method calls this callback function each time it fills a
/// row of the noise map with coherent-noise values.  This callback
/// function has a single integer parameter that contains a count of the
/// rows that have been completed.  It returns void.
///
/// Note that SetBounds() is not defined in the abstract base class; it is
/// only defined in the derived classes.  This is because each model uses
/// a different coordinate system.
#include <libnoise/module/modulebase.h>

namespace noise {

    namespace utils {

        class NoiseMap;


        /// The NoiseMapBuilder::Build() method calls this callback function each
        /// time it fills a row of the noise map with coherent-noise values.
        ///
        /// This callback function has a single integer parameter that contains
        /// a count of the rows that have been completed.  It returns void.  Pass
        /// a function with this signature to the NoiseMapBuilder::SetCallback()
        /// method.
        typedef void(* NoiseMapCallback) (int row);

        class NoiseMapBuilder {

        public:

            /// Constructor.
            NoiseMapBuilder ();

            /// Builds the noise map.
            ///
            /// @pre SetBounds() was previously called.
            /// @pre SetDestNoiseMap() was previously called.
            /// @pre SetSourceModule() was previously called.
            /// @pre The width and height values specified by SetDestSize() are
            /// positive.
            /// @pre The width and height values specified by SetDestSize() do not
            /// exceed the maximum possible width and height for the noise map.
            ///
            /// @post The original contents of the destination noise map is
            /// destroyed.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            /// @throw noise::ExceptionOutOfMemory Out of memory.
            ///
            /// If this method is successful, the destination noise map contains
            /// the coherent-noise values from the noise module specified by
            /// SetSourceModule().
            virtual void Build () = 0;

            /// Returns the height of the destination noise map.
            ///
            /// @returns The height of the destination noise map, in points.
            ///
            /// This object does not change the height in the destination noise
            /// map object until the Build() method is called.
            double GetDestHeight () const {
                return m_destHeight;
            }

            /// Returns the width of the destination noise map.
            ///
            /// @returns The width of the destination noise map, in points.
            ///
            /// This object does not change the height in the destination noise
            /// map object until the Build() method is called.
            double GetDestWidth () const {
                return m_destWidth;
            }

            /// Sets the callback function that Build() calls each time it fills a
            /// row of the noise map with coherent-noise values.
            ///
            /// @param pCallback The callback function.
            ///
            /// This callback function has a single integer parameter that
            /// contains a count of the rows that have been completed.  It returns
            /// void.  Pass a function with this signature to the SetCallback()
            /// method.
            void SetCallback (NoiseMapCallback pCallback);

            /// Sets the destination noise map.
            ///
            /// @param destNoiseMap The destination noise map.
            ///
            /// The destination noise map will contain the coherent-noise values
            /// from this noise map after a successful call to the Build() method.
            ///
            /// The destination noise map must exist throughout the lifetime of
            /// this object unless another noise map replaces that noise map.
            void SetDestNoiseMap (NoiseMap& destNoiseMap) {
                m_pDestNoiseMap = &destNoiseMap;
            }

            /// Sets the source module.
            ///
            /// @param sourceModule The source module.
            ///
            /// This object fills in a noise map with the coherent-noise values
            /// from this source module.
            ///
            /// The source module must exist throughout the lifetime of this
            /// object unless another noise module replaces that noise module.
            void SetSourceModule (const module::Module& sourceModule) {
                m_pSourceModule = &sourceModule;
            }

            /// Sets the size of the destination noise map.
            ///
            /// @param destWidth The width of the destination noise map, in
            /// points.
            /// @param destHeight The height of the destination noise map, in
            /// points.
            ///
            /// This method does not change the size of the destination noise map
            /// until the Build() method is called.
            void SetDestSize (int destWidth, int destHeight) {
                m_destWidth = destWidth;
                m_destHeight = destHeight;
            }

        protected:

            /// The callback function that Build() calls each time it fills a row
            /// of the noise map with coherent-noise values.
            ///
            /// This callback function has a single integer parameter that
            /// contains a count of the rows that have been completed.  It returns
            /// void.  Pass a function with this signature to the SetCallback()
            /// method.
            NoiseMapCallback m_pCallback;

            /// Height of the destination noise map, in points.
            int m_destHeight;

            /// Width of the destination noise map, in points.
            int m_destWidth;

            /// Destination noise map that will contain the coherent-noise values.
            NoiseMap* m_pDestNoiseMap;

            /// Source noise module that will generate the coherent-noise values.
            const module::Module* m_pSourceModule;

        };
    }
}

#endif //CALENHAD_NOISEMAPBUILDER_H
