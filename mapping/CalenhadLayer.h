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
        class GradientLegend;
    }
    namespace module {
        class Module;
    }
    namespace model {
        class Sphere;
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
    void setGradient (noise::utils::GradientLegend* gradient);
protected:
    double _angularResolution;
    noise::module::Module* _source;
    noise::utils::GradientLegend* _gradient;
    Marble::ViewportParams* _viewport;
    noise::model::Sphere* _sphere;

};


#endif //CALENHAD_CALENHADLAYER_H
