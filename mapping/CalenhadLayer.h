//
// Created by martin on 26/01/17.
//

#ifndef CALENHAD_CALENHADLAYER_H
#define CALENHAD_CALENHADLAYER_H

#include <marble/LayerInterface.h>
#include <marble/GeoDataLatLonBox.h>
#include <memory>
#include <marble/TileId.h>
#include <marble/GeoDataLatLonAltBox.h>
#include "../qmodule/QModule.h"

class QColorGradient;

namespace icosphere {
    class Icosphere;
}

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

class RenderJob;

using namespace Marble;

class CalenhadLayer : public QObject, public Marble::LayerInterface {
    Q_OBJECT

public:
    static const int INITIAL_STEP = 64;
    CalenhadLayer (QModule* source);
    virtual ~CalenhadLayer();
    QStringList renderPosition() const override;
    virtual bool render	(Marble::GeoPainter* painter, Marble::ViewportParams* viewport, const QString & renderPos, Marble::GeoSceneLayer* layer) override;
    int render (Marble::GeoPainter* painter, Marble::ViewportParams* viewport);
    void setGradient (noise::utils::GradientLegend* gradient);
    public slots:
    void rescale();
    QImage* overview();

signals:
    void imageRefreshed();
    void overviewRendered (const QImage& image);
    void progress (const double&);
    void renderingStarted();
    void renderingFinished();

protected:
    int render (Marble::GeoPainter* painter, Marble::ViewportParams* viewport, const int& offset);
    double _angularResolution;
    QModule* _source;
    QImage* _overview;
    noise::utils::GradientLegend* _gradient;
    noise::model::Sphere* _sphere;
    int _step;
    int _done;

    void renderMainMap (GeoPainter* painter, ViewportParams* viewport, bool rehearse = false);

    void renderOverview ();

    int _toDo;

    bool _finished;
};


#endif //CALENHAD_CALENHADLAYER_H
