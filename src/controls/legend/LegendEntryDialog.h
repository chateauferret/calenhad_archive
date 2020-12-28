//
// Created by martin on 19/02/18.
//

#ifndef CALENHAD_LEGENDENTRYDIALOG_H
#define CALENHAD_LEGENDENTRYDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>

namespace calenhad {
    namespace expressions {
        class ExpressionWidget;
    }
    namespace controls {
        namespace legend {
            class QColorButton;
            class LegendEditorSlider;
            class LegendEntryDialog : public QDialog {

            public:
                LegendEntryDialog (QWidget* parent = 0);
                ~LegendEntryDialog() override;
                QColor color();
                QString index();
                bool isDelete();
                void setColor (const QColor& color);
                void setIndex (const QString& index) const;
                void preventDelete (const bool& prevented);

            public slots:
                void selectColor();

            protected:
                calenhad::expressions::ExpressionWidget* _indexEdit;
                QColorButton* _colorButton;
                QColorDialog* _dialog;
                QCheckBox* _deleteCheck;

                void showEvent (QShowEvent* event) override;
                LegendEditorSlider* _slider;
            };
        }
    }
}



#endif //CALENHAD_LEGENDENTRYDIALOG_H
