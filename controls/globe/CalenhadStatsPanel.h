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
    namespace qmodule {
        class Module;
    }
    namespace controls {
        namespace globe {

            class CalenhadGlobeWidget;

            class CalenhadStatsPanel : public QWidget {
            public:
                CalenhadStatsPanel (calenhad::qmodule::Module* source, CalenhadGlobeWidget* parent = 0);

                virtual ~CalenhadStatsPanel ();

                void showEvent (QShowEvent* e) override;

            public slots:
                void refresh ();

            protected:
                CalenhadGlobeWidget* dialog;
                QLabel* _mapExtremesLabel, * _worldExtremesLabel;
                QLabel* _worldMeanLabel;
                calenhad::controls::globe::HypsographyWidget* _hypsography;
                calenhad::qmodule::Module* _source;


                QLabel*  _renderTimeLabel, * _renderSizeLabel;
            };
        }
    }
}


#endif //CALENHAD_CALENHADSTATSTAB_H
