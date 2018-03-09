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
#include <QtWidgets/QCheckBox>
#include "Node.h"


namespace calenhad {
    namespace legend {
        class Legend;
    }
    namespace nodeedit {
        class NodeBlock;
        class Port;
    }
    namespace notification {
        class QNotificationFactory;
    }
    namespace controls {
        namespace globe {
            class CalenhadMapView;
            class CalenhadGlobeDialog;
            class CalenhadStatsPanel;
        }
    }
    namespace qmodule {
        class RangeFinder;
        class Module : public Node {
        Q_OBJECT




        Q_ENUMS (ModuleType)
        public:
            Module (const QString& nodeType, QWidget* parent = 0);

            virtual ~Module ();
            QString label();
            QString description();
            virtual QString glsl();
            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;
            calenhad::controls::globe::CalenhadMapView* preview();
            static int seed;

            // this is called by renderers before any rendering takes place, to allow the module to precalculate anything required for rendering.
            // If it returns false, rendering does not take place.
            virtual bool generateMap();

            void setModel (calenhad::pipeline::CalenhadModel* model) override;

            void setLegend (calenhad::legend::Legend* legend);
            Node* sourceModule (int portIndex);
            calenhad::legend::Legend* legend ();
            virtual void addInputPort (const int& index, const int& portType, const QString& name);
            void showContextMenu (const QPoint& point);
            bool isComplete() override;
            bool range (double& min, double& max);
            QMap<unsigned, calenhad::nodeedit::Port*> inputs();
        public slots:
            void setupPreview ();
            void showGlobe ();
            void rendered (const bool& success);
            void parameterChanged() override;

        protected:

            virtual void contextMenuEvent (QContextMenuEvent* e) override;



            void initialise () override;

            QFormLayout* _previewLayout;
            calenhad::controls::globe::CalenhadMapView* _preview;

            int _previewIndex;
            calenhad::legend::Legend* _legend;

            QMenu* _contextMenu;
            calenhad::controls::globe::CalenhadGlobeDialog* _globe;


            int _statsIndex;
            QDialog* _stats;

        };
    }
}


#endif //CALENHAD_QMODULE_H
