//
// Created by martin on 16/11/16.
//

#ifndef CALENHAD_QMODULE_H
#define CALENHAD_QMODULE_H


#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QTextEdit>
#include <QUuid>

#include <memory>
#include <controls/globe/CalenhadMapView.h>
#include "QNode.h"


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
        namespace globe {
            class CalenhadMapView;
            class CalenhadGlobeDialog;
        }
    }
    namespace qmodule {
        class RangeFinder;
        class QModule : public QNode {
        Q_OBJECT
        Q_ENUMS (ModuleType)
        public:
            QModule (const QString& nodeType, RangeFinder* rangeFinder, int inputs = 0, QWidget* parent = 0);

            virtual ~QModule ();

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;
            calenhad::controls::globe::CalenhadMapView* preview();
            static int seed;

            // this is called by renderers before any rendering takes place, to allow the module to precalculate anything required for rendering.
            // If it returns false, rendering does not take place.
            virtual bool generateMap ();

            void setModel (calenhad::pipeline::CalenhadModel* model) override;

            void setLegend (calenhad::legend::Legend* legend);
            QNode* sourceModule (int portIndex);
            calenhad::legend::Legend* legend ();

            void showContextMenu (const QPoint& point);
            bool isComplete() override;
            bool range (double& min, double& max);

        public slots:
            void setupPreview ();
            void invalidate () override;
            void showGlobe ();
        protected:

            virtual void contextMenuEvent (QContextMenuEvent* e) override;

            virtual void addInputPorts ();

            void initialise () override;

            QFormLayout* _previewLayout;
            calenhad::controls::globe::CalenhadMapView* _preview;

            int _previewIndex;
            calenhad::legend::Legend* _legend;

            QMenu* _contextMenu;
            calenhad::controls::globe::CalenhadGlobeDialog* _globe;
            RangeFinder* _rangeFinder;


        };
    }
}


#endif //CALENHAD_QMODULE_H
