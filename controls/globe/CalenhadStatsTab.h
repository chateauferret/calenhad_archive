//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_CALENHADSTATSTAB_H
#define CALENHAD_CALENHADSTATSTAB_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <qwt/qwt_plot.h>
#include <QtWidgets/QComboBox>

class QModule;

namespace calenhad {
    namespace controls {
        namespace globe {

            class CalenhadGlobeDialog;

            class CalenhadStatsTab : public QWidget {
            public:
                CalenhadStatsTab (CalenhadGlobeDialog* parent);

                virtual ~CalenhadStatsTab ();

                void showEvent (QShowEvent* e) override;

            protected:
                CalenhadGlobeDialog* _globe;
                QLabel* _extremesMapLabel, * _extremesGlobeLabel;
                QLabel* _renderTimeLabel, * _renderTimePerPixelLabel;
                QLabel* _pixelsInMapLabel;
                QLabel* _whenGeneratedLabel;
                QwtPlot* _hypsography;
            };
        }
    }
}


#endif //CALENHAD_CALENHADSTATSTAB_H
