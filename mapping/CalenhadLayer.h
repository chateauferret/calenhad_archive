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
#include "../pipeline/GlobeRenderJob.h"

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

class ImageRenderJob;

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
    void refresh (std::shared_ptr<GlobeBuffer> buffer);
    void populate();
    void updateBuffer (std::shared_ptr<GlobeBuffer> buffer);

signals:
    void imageRefreshed();
    void overviewRendered (const QImage& image);
    void progress (const double&);
    void abandonJobs();

protected:

    QModule* _source;
    QImage* _overview;
    noise::utils::GradientLegend* _gradient;
    std::shared_ptr<GlobeBuffer> _buffer;

    int _done;

    void renderOverview ();

    int _toDo;

    bool _finished;


    QMutex mutex;
    bool _globeChanged;
    ViewportParams* _viewport;

    GeoDataLatLonAltBox _box;
};


#endif //CALENHAD_CALENHADLAYER_H
