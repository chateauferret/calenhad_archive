//
// Created by martin on 10/10/17.
//

#ifndef CALENHAD_GLOBESCALEWIDGET_H
#define CALENHAD_GLOBESCALEWIDGET_H

#include <QtWidgets/QWidget>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_scale_engine.h>
#include <CalenhadServices.h>
#include <GeographicLib/Geodesic.hpp>
#include "../../preferences/PreferencesService.h"

namespace calenhad {
    namespace controls {
        namespace globe {
            class CalenhadMapView;
            class GlobeScaleWidget : public QwtScaleWidget {
            public:
                GlobeScaleWidget (CalenhadMapView* _globe, QWidget* parent = 0, const double& radius = CalenhadServices::preferences() -> calenhad_default_planet_radius);
                virtual ~GlobeScaleWidget();
                void setOrientation (Qt::Orientation orientation);
            protected:
                QwtScaleDraw* _draw;
                QwtLinearScaleEngine* _scaleEngine;

                void paintEvent (QPaintEvent* e) override;

                Qt::Orientation _orientation;
                CalenhadMapView*  _globe;
                double _radius;
                GeographicLib::Geodesic*  _geodesic;
                double _metresPerUnit;
            };

        }
    }
}


#endif //CALENHAD_GLOBESCALEWIDGET_H
