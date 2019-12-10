//
// Created by martin on 07/02/18.
//

#ifndef CALENHAD_ALTITUDEMAPPINGDIALOG_H
#define CALENHAD_ALTITUDEMAPPINGDIALOG_H


#include <QtWidgets/QWidget>
#include "AltitudeMapping.h"
#include <QDialog>
namespace calenhad {

    namespace expressions {
        class ExpressionWidget;
    }

    namespace controls {
        namespace altitudemap {
            class AltitudeMappingDialog : public QDialog {
            Q_OBJECT
            public:
                AltitudeMappingDialog (QWidget* parent = 0);
                virtual ~AltitudeMappingDialog();
                void setMapping (const AltitudeMapping& mapping);
                AltitudeMapping mapping();
                bool hasErrors();
                int index();
                void setIndex (const int& index);
            protected:
                AltitudeMapping _mapping;
                calenhad::expressions::ExpressionWidget* _from, * _to;
                int _index;

                void focusOutEvent (QFocusEvent* event) override;

                void showEvent (QShowEvent* event) override;
            };
        }
    }
}

#endif //CALENHAD_ALTITUDEMAPPINGDIALOG_H
