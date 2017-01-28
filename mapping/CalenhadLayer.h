//
// Created by martin on 26/01/17.
//

#ifndef CALENHAD_CALENHADLAYER_H
#define CALENHAD_CALENHADLAYER_H

#include <QString>
#include <QStringList>
#include <marble/LayerInterface.h>
#include <marble/GeoDataLatLonBox.h>

class QColorGradient;

namespace noise {
    namespace utils {
        class Gradient;
    }
    namespace module {
        class Module;
    }
}

namespace Marble {
    class LayerInterface;
    class GeoPainter;
    class ViewportParams;
    class GeoSceneLayer;
    class GeoDataLatLonBox;
}

class CalenhadLayer : public Marble::LayerInterface {

public:
    CalenhadLayer (noise::module::Module* source);
    virtual ~CalenhadLayer();
    QStringList renderPosition() const override;
    virtual bool render	(Marble::GeoPainter* painter, Marble::ViewportParams* viewport, const QString & renderPos, Marble::GeoSceneLayer* layer) override;
    bool render (Marble::GeoPainter* painter, Marble::GeoDataLatLonBox box);
    void setGradient (QColorGradient* gradient);
protected:
    double _angularResolution;
    noise::module::Module* _source;
    QColorGradient* _gradient;
    Marble::ViewportParams* _viewport;


};


#endif //CALENHAD_CALENHADLAYER_H
