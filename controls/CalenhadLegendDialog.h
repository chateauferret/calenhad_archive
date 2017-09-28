//
// Created by martin on 28/09/17.
//

#ifndef CALENHAD_CALENHADLEGENDDIALOG_H
#define CALENHAD_CALENHADLEGENDDIALOG_H
#include <QDialog>

namespace calenhad {
    namespace legend {
        class LegendManager;
        class Legend;
    }
    namespace controls {
        class CalenhadLegendDialog : public QDialog {
        Q_OBJECT
        public:

            CalenhadLegendDialog (QWidget* parent);
            void reject() override;
            void accept() override;

            signals:
            void legendChanged (calenhad::legend::Legend* legend);

        protected:

            calenhad::legend::LegendManager* _legendManager;


            void showEvent (QShowEvent* event);
        };
    }
}


#endif //CALENHAD_CALENHADLEGENDDIALOG_H
