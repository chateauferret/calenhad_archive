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
#include <mapping/CalenhadMapWidget.h>
#include "Node.h"


namespace calenhad {
    namespace legend {
        class Legend;
    }
    namespace nodeedit {
        class NodeBlock;
        class Port;
    }

    namespace controls {
        namespace globe {
            class CalenhadMapView;
            class CalenhadGlobeDialog;
            class CalenhadStatsPanel;
        }
    }
    namespace qmodule {
        class Module : public Node {
        Q_OBJECT

            Q_ENUMS (ModuleType)

        public:
            Module (const QString& nodeType, const bool& suppressRender = false, QWidget* parent = 0);
            virtual ~Module ();
            QString label();
            QString description();
            virtual QString glsl();
            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomElement& element) override;
            mapping::CalenhadMapWidget* preview();
            static int seed;

            // this is called by renderers before any rendering takes place, to allow the module to precalculate anything required for rendering.
            // If it returns false, rendering does not take place.
            virtual bool generateMap();

            void setModel (calenhad::pipeline::CalenhadModel* model) override;
            void setLegend (calenhad::legend::Legend* legend);
            Node* sourceModule (int portIndex);
            calenhad::legend::Legend* legend ();
            void addInputPort (const unsigned int& index, const int& portType, const QString& name, const QString& label, const double& defaultValue = 0.0, const bool& required = false);

            void showContextMenu (const QPoint& point);
            bool isComplete() override;
            bool range (double& min, double& max);
            QMap<unsigned, calenhad::nodeedit::Port*> inputs();

            void initialise () override;
            void connectMenu (QMenu* menu, calenhad::nodeedit::Port* port);
            calenhad::nodeedit::Port* output();
            void addPort (calenhad::nodeedit::Port* port, const unsigned& index = 0);

            bool renderSuppressed ();

            QVector<nodeedit::Port*> ports ();
        public slots:
            void setupPreview ();
            void showGlobe ();
            void rendered (const bool& success);
            void parameterChanged() override;

        protected:

            virtual void contextMenuEvent (QContextMenuEvent* e) override;
            virtual void addInputPorts();


            bool _suppressRender;
            QFormLayout* _previewLayout;
            calenhad::mapping::CalenhadMapWidget* _preview;
            int _previewIndex;
            calenhad::legend::Legend* _legend;

            QMenu* _connectMenu;
            QMenu* _contextMenu;
            calenhad::controls::globe::CalenhadGlobeDialog* _globe;
            QVector<calenhad::nodeedit::Port*> _ports;
            calenhad::nodeedit::Port* _output;
            QMap<unsigned, calenhad::nodeedit::Port*> _inputs;

            int _statsIndex;
            QDialog* _stats;


            QString _shownParameter;
            bool _editable;

        };
    }
}


#endif //CALENHAD_QMODULE_H
