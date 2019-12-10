//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_CALENHADSTATSTAB_H
#define CALENHAD_CALENHADSTATSTAB_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <qwt/qwt_plot.h>
#include <QtWidgets/QComboBox>
#include "HypsographyWidget.h"


namespace calenhad {
    namespace module {
        class Module;
    }
    namespace controls {
        namespace globe {

            class CalenhadGlobeWidget;

            class CalenhadStatsPanel : public QWidget {
            public:
                explicit CalenhadStatsPanel (calenhad::module::Module* source, CalenhadGlobeWidget* parent = nullptr);

                ~CalenhadStatsPanel () override;

                void showEvent (QShowEvent* e) override;

            public slots:
                void refresh ();

            protected:
                CalenhadGlobeWidget* dialog;
                QLabel* _mapExtremesLabel, * _worldExtremesLabel;
                QLabel* _worldMeanLabel;
                calenhad::controls::globe::HypsographyWidget* _hypsography;
                calenhad::module::Module* _source;


                QLabel*  _renderTimeLabel, * _renderSizeLabel;
            };
        }
    }
}


#endif //CALENHAD_CALENHADSTATSTAB_H
