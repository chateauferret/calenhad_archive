//
// Created by ferret on 16/01/2021.
//

#ifndef CALENHAD_RASTER_H
#define CALENHAD_RASTER_H

#include <geoutils.h>
#include "projection/Projection.h"

namespace calenhad {
    namespace mapping {

        struct Extent {
        public:
            Extent();
            ~Extent();
            //void setBounds (geoutils::Geolocation bounds [4]);
            //void setBounds ();
            geoutils::Geolocation _bounds [4] ;
            projection::Projection* _projection;
        } __attribute__((aligned(128)));

        struct Raster {
        public:
            explicit Raster (QImage* image);
            Raster (QImage* image, const Extent& extent);
            ~Raster();
            QImage* image();
            void setImage (QImage* image);
            Extent extent();
            void setExtent (const Extent& extent);
        private:
            QImage* _image;
            Extent _extent;             // nullptr means world equirectangular

        } __attribute__((aligned(128)));

    }
}


#endif //CALENHAD_RASTER_H
