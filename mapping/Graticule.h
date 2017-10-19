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
            void getIntersections (const QPair<double, double>& g, const double& interval, QSet<QPair<double, double>>& result);
            void drawGraticuleIntersection (QPainter& p, const QPair<double, double>& g, const int& level);
            void makeGraticule (QPainter& p, const int& level, QSet<QPair<double, double>>& result);
            QPen _majorPen, _minorPen;


            double subdivisions (const int& i);

        };
    }
}



#endif //CALENHAD_GRATICULE_H
