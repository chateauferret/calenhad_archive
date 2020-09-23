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
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"
#include <memory>
#include <QtWidgets/QCheckBox>

#include "Node.h"


namespace calenhad {
    namespace grid {
        class Extent;
    }

    namespace legend {
        class Legend;
    }
    namespace nodeedit {
        class NodeBlock;
        class Port;
    }

    namespace module {
        class Module : public Node {
        Q_OBJECT


            Q_ENUMS (ModuleType)

        public:
            explicit Module (const QString& nodeType, QWidget* parent = nullptr);
            ~Module () override;
            QString label();
            QString description();
            virtual QString glsl();
            virtual void fetch (calenhad::grid::CubicSphere* buffer);

            void setModel (calenhad::pipeline::CalenhadModel* model) override;
            void addInputPort (const unsigned int& index, const int& portType, const QString& name, const QString& label, const double& defaultValue = 0.0, const bool& required = false);
            bool isComplete() override;

            QMap<unsigned, calenhad::nodeedit::Port*> inputs();

            void connectMenu (QMenu* menu, calenhad::nodeedit::Port* port);
            calenhad::nodeedit::Port* output();
            void addPort (calenhad::nodeedit::Port* port, const unsigned& index = 0);
            QVector<nodeedit::Port*> ports ();
            virtual bool isComputed();

        public slots:
            void parameterChanged() override;
            void invalidate() override;
            void showGlobe();

        protected:

            QSet<Module*> dependants();
            QMenu* _connectMenu;
            QVector<calenhad::nodeedit::Port*> _ports;
            calenhad::nodeedit::Port* _output;
            QMap<unsigned, calenhad::nodeedit::Port*> _inputs;

            QString _shownParameter;

            bool _valid;


            void copy (grid::CubicSphere* buffer);
        };
    }
}


#endif //CALENHAD_QMODULE_H
