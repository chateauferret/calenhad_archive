//
// Created by martin on 16/11/16.
//

#ifndef CALENHAD_QMODULE_H
#define CALENHAD_QMODULE_H


#include <libnoise/module/module.h>
#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QTextEdit>
#include <QUuid>

#include <memory>
#include "QNode.h"

namespace Marble {
    class GeoDataLatLonAltBox;
}

namespace calenhad {
    namespace legend {
        class Legend;
    }
    namespace nodeedit {
        class QNodeBlock;
        class QNEPort;
    }
    namespace notification {
        class QNotificationFactory;
    }
    namespace controls {
        class QNoiseMapViewer;
    }
    namespace qmodule {

        class QModule : public QNode {
        Q_OBJECT
        Q_ENUMS (ModuleType)
        public:
            QModule (noise::module::Module* m, QWidget* parent = 0);

            virtual ~QModule ();

            // don't want a copy constructor because subclass implementations will have to call initialise()
            virtual QModule* clone () = 0;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

            virtual QString nodeType () override = 0;

            static int seed;
            static noise::NoiseQuality noiseQuality;

            virtual noise::module::Module* module ();

            // this is called by renderers before any rendering takes place, to allow the module to precalculate anything required for rendering.
            // If it returns false, rendering does not take place.
            virtual bool generateMap ();

            void setModel (calenhad::pipeline::CalenhadModel* model) override;

            void setLegend (calenhad::legend::Legend* legend);

            calenhad::legend::Legend* legend ();

            std::shared_ptr<QImage> overview ();
            bool isComplete() override;
        public slots:

            void invalidate () override;

        protected:

            virtual void addInputPorts ();

            void initialise () override;

            QFormLayout* _previewLayout;
            calenhad::controls::QNoiseMapViewer* _preview;
            noise::module::Module* _module;

            int _previewIndex;
            calenhad::legend::Legend* _legend;

            bool _renderRequested;

        protected slots:

            void setupPreview ();


        };
    }
}


#endif //CALENHAD_QMODULE_H
