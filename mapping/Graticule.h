//
// Created by martin on 16/10/17.
//

#ifndef CALENHAD_GRATICULE_H
#define CALENHAD_GRATICULE_H
#include <QWidget>
#include <QPaintEvent>
#include <QPen>

namespace calenhad {
    namespace mapping {
        class CalenhadMapWidget;
        class Graticule : public QWidget {
        public:
            Graticule (CalenhadMapWidget* globe);
            virtual ~Graticule();

            double pitch (const int& i);
            void drawGraticule (QPainter& p);

            QPen majorPen ();

            QPen minorPen ();
            void setPens (QPen majorPen, QPen minorPen);

            void setDensity (const int& density);
            bool visible () const;
            void setVisible (bool visible);
            int density () const;

        protected:
            bool _visible;
            int _density;
            CalenhadMapWidget* _globe;
            void getIntersections (const QPair<double, double>& g, const double& interval);
            void drawLatitudeIntersection (QPainter& p, const QPair<double, double>& g, const int& level);
            void drawLongitudeIntersection (QPainter& p, const QPair<double, double>& g, const int& level);
            void makeGraticule (QPainter& p, const int& level);
            QPen _majorPen, _minorPen;
            QLineF _bottom, _left, _insetTop, _insetRight;
            QSet<QPair<double, double>> _intersections;
            double subdivisions (const int& i);

            void drawLatitude (QPainter& p, const double& lat, const int& level);
            void drawLongitude (QPainter& p, const double& lon, const int& level);

            double _centralLat, _centralLon;

            void placeLatitudeLabel (QPainter& p, const double& lat);

            void placeLongitudeLabel (QPainter& p, const double& lon);

            QList<double> _latitudesLabelled, _longitudesLabelled;

            QString longitudeLabel (const double& lon);

            QString latitudeLabel (const double& lat);
        };
    }
}



#endif //CALENHAD_GRATICULE_H
