//
// Created by martin on 31/05/17.
//

#ifndef CALENHAD_GRADIENTLEGENDWIDGET_H
#define CALENHAD_GRADIENTLEGENDWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QCheckBox>

namespace calenhad {
    namespace controls {
        namespace legend {
            class LegendEditor;
        }
    }

    namespace legend {
        class Legend;
        class LegendService;
        class LegendWidget : public QWidget {
        Q_OBJECT

        public:
            LegendWidget (Legend* legend, QWidget* parent = 0);

            virtual ~LegendWidget ();

            QLineEdit* _legendNameBox;
            calenhad::controls::legend::LegendEditor* _legendEditor;
            QCheckBox* _legendInterpolateCheck;
            QTextEdit* _legendNotesBox;

            void updateLegend ();

            void showEvent (QShowEvent* event) override;

            QList<LegendEntry> entries ();

            void focusNameField ();

            const bool& isInterpolated ();

            Legend* legend ();

        signals:

            void nameChanged (QString);

            void notesChanged (QString);

            void iconChanged (QIcon);

            void legendChanged (QList<LegendEntry>);

        protected:
            LegendService* _service;
            Legend* _legend;

        protected slots:

            void checkName ();

            void updateName ();


        };
    }
}

#endif //CALENHAD_GRADIENTLEGENDWIDGET_H
