//
// Created by martin on 26/01/17.
//

#ifndef CALENHAD_CALENHADLAYER_H
#define CALENHAD_CALENHADLAYER_H

#include <marble/LayerInterface.h>
#include <marble/GeoDataLatLonBox.h>
#include <memory>
#include <vector>
#include <marble/TileId.h>
#include <marble/GeoDataLatLonAltBox.h>
#include <QtCore/QMutex>
#include "../qmodule/QModule.h"
//#include "../pipeline/GlobeRenderJob.h"

#include "RenderPoint.h"

class QColorGradient;

namespace icosphere {
    class Icosphere;
}

namespace noise {
    namespace utils {
        class Legend;
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
namespace calenhad {
    namespace qmodule {
        class QModule;
    }
    namespace legend {
        class Legend;
    }
    namespace pipeline {
        class ImageRenderJob;
    }
    namespace mapping {
        using namespace Marble;
        typedef std::vector<RenderPoint> RenderLine;
        typedef std::vector<RenderLine> RenderBuffer;

        class CalenhadLayer : public QObject, public Marble::LayerInterface {
        Q_OBJECT

        public:
            static const int INITIAL_STEP = 64;

            CalenhadLayer (calenhad::qmodule::QModule* source);

            virtual ~CalenhadLayer ();

            QStringList renderPosition () const override;

            virtual bool
            render (Marble::GeoPainter* painter, Marble::ViewportParams* viewport, const QString& renderPos, Marble::GeoSceneLayer* layer) override;

            int render (Marble::GeoPainter* painter);

            calenhad::legend::Legend* legend ();

        public slots:

            void rescale ();
            void restart();
            void refresh ();

            void populate ();

        signals:

            void imageRefreshed ();

            void overviewRendered (const QImage& image);

            void progress (const double&);

            void complete ();

            void abandonJobs ();

            void renderingStart ();

            void renderingFinished ();

        protected:
            void renderOverview ();

            calenhad::qmodule::QModule* _source;
            QImage* _overview;
            std::shared_ptr<RenderBuffer> _buffer;
            QTimer* _timer;
            QMutex _mutex;
            RenderBuffer::iterator _scanline, _renderline;
            noise::model::Sphere* _sphere;
            ViewportParams* _viewport;

            // these fields are used to populate the statistics tab on the CalenhadGlobe properties dialog
            double _minimum;
            double _maximum;
            time_t _start;
            int _count;

            // remember the scope of the viewport so that we can tell whether the view has changed
            GeoDataLatLonBox _box;
            RenderBuffer::iterator i;

            void makeBuffer (ViewportParams* viewport);
            bool _restart;
        protected slots:

            void gatherStats ();


        };
    }
}

#endif //CALENHAD_CALENHADLAYER_H
