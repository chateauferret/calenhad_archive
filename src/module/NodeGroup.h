//
// Created by martin on 06/06/17.
//

#ifndef CALENHAD_QWIDGETGROUP_H
#define CALENHAD_QWIDGETGROUP_H

#include "Node.h"
#include <QDomElement>

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace module {
        class Node;
        class NodeGroup {
        public:
            explicit NodeGroup (calenhad::pipeline::CalenhadModel* model);
            virtual ~NodeGroup ();

            void inflate (const QDomElement& element);
            void serialize (QDomElement& element);
            void setColors (const QColor& color, const QColor& selectedColor, const QColor& borderColor, const QColor& selectedBorderColor);
            void setName (const QString& name);
            void setDocumentation (const QString& documentation);
            void setModel (calenhad::pipeline::CalenhadModel* model);
            QList<Module*> modules();

            QString name();
            QString documentation();
            QColor color();
            QColor borderColor();
            QColor selectedColor();
            QColor selectedBorderColor();



        protected:
            QColor _color, _selectedColor, _borderColor, _selectedBorderColor;
            QString _name, _documentation;
            calenhad::pipeline::CalenhadModel* _model;

        };
    }
}


#endif //CALENHAD_QWIDGETGROUP_H
