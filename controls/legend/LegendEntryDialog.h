//
// Created by martin on 19/02/18.
//

#ifndef CALENHAD_LEGENDENTRYDIALOG_H
#define CALENHAD_LEGENDENTRYDIALOG_H

#include <QtWidgets/QDialog>

namespace calenhad {
    namespace expressions {
        class ExpressionWidget;
    }
    namespace controls {
        namespace legend {
            class QColorButton;
            class LegendEntryDialog : public QDialog {

            public:
                LegendEntryDialog (QWidget* parent = 0);
                ~LegendEntryDialog();
                QColor color();
                QString index();
                void setColor (const QColor& color);
                void setIndex (const QString& index) const;


            public slots:
                void selectColor();

            protected:
                calenhad::expressions::ExpressionWidget* _indexEdit;
                QColorButton* _colorButton;
                QColorDialog* _dialog;
            };
        }
    }
}



#endif //CALENHAD_LEGENDENTRYDIALOG_H
