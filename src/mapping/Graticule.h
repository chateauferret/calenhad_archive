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
        class AbstractMapWidget;
        class Graticule : public QWidget {
        public:
            explicit Graticule (AbstractMapWidget* globe);
            ~Graticule() override;

            double pitch (const int& i);
            void drawGraticule (QPainter& p);

            QPen majorPen ();

            QPen minorPen ();
            void setPens (QPen majorPen, QPen minorPen);

            void setDensity (const int& density);
            bool visible () const;
            void setVisible (bool visible) override;
            int density () const;

        protected:
            bool _visible;
            int _density;
            AbstractMapWidget* _globe;
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

            QString longitudeLabel (const double& lon, const bool& major = true);

            QString latitudeLabel (const double& lat, const bool& major = true);

            void drawLabel (QPainter& p, QPointF point, const QString& text);

            void drawLatitudeLabel (QPainter& p, QPointF point, const QString& text);

            void drawLongitudeLabel (QPainter& p, QPointF point, const QString& text);
        };
    }
}



#endif //CALENHAD_GRATICULE_H
