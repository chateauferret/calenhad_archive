//
// Created by martin on 31/03/17.
//

#ifndef CALENHAD_ALTITUDEMAPEDITOR_H
#define CALENHAD_ALTITUDEMAPEDITOR_H


#include <QtWidgets/QDialog>
#include <QtCore/QMap>
#include <qwt/qwt_series_data.h>
#include <QtWidgets/QComboBox>
#include "AltitudeMapConstants.h"

namespace calenhad {
    namespace controls {
        namespace altitudemap {

            class AltitudeMapPlot;

            class AltitudeMapEditor : public QDialog {
            Q_OBJECT

            public:
                AltitudeMapEditor (QWidget* parent = 0);

                ~AltitudeMapEditor ();

                QVector<QPointF> getEntries ();

                CurveType curveType ();

                void setCurveType (CurveType curveType);

            public slots:

                void setEntries (const QVector<QPointF>& entries);

                void accept ();

                void reject ();

                void curveTypeSelected (int index);

            protected:
                AltitudeMapPlot* _plot;
                QComboBox* _curveTypeCombo;

                virtual void showEvent (QShowEvent* event) override;

                QMap<QString, CurveType> _curveTypeOptions;


            };
        }
    }
}



#endif //CALENHAD_ALTITUDEMAPEDITOR_H
