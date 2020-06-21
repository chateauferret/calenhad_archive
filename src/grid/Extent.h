//
// Created by ferret on 19/06/2020.
//

#ifndef MESSAGES_EXTENT_H
#define MESSAGES_EXTENT_H

#include <src/module/Module.h>
#include "Extent.h"
#include "geoutils.h"
#include "Bounds.h"

namespace calenhad {

    namespace module {
        class Module;
    }

    namespace grid {
        class Bounds;

        class Extent {
        public:
            explicit Extent (calenhad::module::Module* module);
            explicit Extent (Extent* other);
            virtual ~Extent();

            virtual float getValue (const geoutils::Geolocation& geolocation);

            // fill the grid of this extent with values from the given module
            virtual void compute();
            int setBounds (const calenhad::grid::Bounds& bounds);
            float* buffer();
            void save();
            Bounds bounds();
            calenhad::module::Module *module();

            size_t rasterHeight() const;

            int resolution() const;

        protected:
            int _resolution = 11;       // the render texture height is two to the power of this number, and the width is twice that
            float* _buffer;
            calenhad::module::Module* _module;
            calenhad::grid::Bounds _bounds;
        };

    }
}


#endif //MESSAGES_EXTENT_H
