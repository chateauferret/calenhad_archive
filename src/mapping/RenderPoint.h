//
// Created by martin on 18/07/17.
//

#ifndef CALENHAD_RENDERPOINT_H
#define CALENHAD_RENDERPOINT_H


#include <QtGui/QColor>
#include <QPoint>
#include <marble/ViewportParams.h>
#include <marble/GeoDataCoordinates.h>

namespace calenhad {
    namespace mapping {


        class RenderPoint {
        public:
            RenderPoint (const QPoint& p, Marble::ViewportParams* viewport);

            const QPoint& point ();

            void populate (const double& value, const QColor& color);

            double _latDegrees, _lonDegrees, _latRadians, _lonRadians;

        protected:
            QPoint _point;

            QColor _color;
            double _value;
        public:
            const QColor& getColor () const;

            double getValue () const;

            bool isValid () const;

            bool isReady () const;

        protected:
            bool _valid, _ready;
            Marble::ViewportParams* _viewport;
        };
    }
}

#endif //CALENHAD_RENDERPOINT_H
