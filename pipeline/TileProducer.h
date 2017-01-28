//
// Created by martin on 11/01/17.
//

#ifndef CALENHAD_TILEPRODUCER_H
#define CALENHAD_TILEPRODUCER_H


#include <QtGui/QImage>
#include <memory>

namespace geoutils {
    class Geolocation;
}

namespace noise {
    namespace module {
        class Module;
    }
}

class RenderJob;

class TileProducer {

public:    TileProducer();
    virtual ~TileProducer();

    static constexpr double PI = 3.141592653589793;
    static constexpr double TWO_PI = 6.283185307179586;
    static constexpr double HALF_PI = 1.570796326794896;
    static constexpr unsigned DEFAULT_SIZE = 256;
    static constexpr unsigned MAX_ZOOM = 24;
    double _width_rads = TWO_PI;
    double _height_rads = PI;
    int _zoom = 1, _size = 256;
    static int powers (int index);
    bool fetchTile (std::shared_ptr<QImage> tile, const unsigned int& x, const unsigned int& y, const unsigned int& z);
    void setModule (noise::module::Module* module);
    void setImageSize (int size);

protected:
    int _xcount, _ycount;
    noise::module::Module* _module;
    geoutils::Geolocation getNWCorner (const unsigned int& x, const unsigned int& y) const;
    geoutils::Geolocation getSECorner (const unsigned int& x, const unsigned int& y) const;

    RenderJob* makeRenderJob (const unsigned int& x, const unsigned int& y, const unsigned int& z);
};




#endif //CALENHAD_TILEPRODUCER_H
