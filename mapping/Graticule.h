//
// Created by martin on 16/10/17.
//

#ifndef CALENHAD_GRATICULE_H
#define CALENHAD_GRATICULE_H
#include <QWidget>
#include <QPaintEvent>

namespace calenhad {
    namespace mapping {
        class CalenhadMapWidget;
        class Graticule : public QWidget {
        public:
            Graticule (CalenhadMapWidget* globe);
            virtual ~Graticule();

            double pitch (const int& i);
            void drawGraticule (QPainter& p);

            bool visible () const;
            void setVisible (bool visible);
            int density () const;
            void setDensity (int density);
        protected:
            bool _visible;
            int _density;
            CalenhadMapWidget* _globe;
            void getIntersections (const QPair<double, double>& g, const double& interval, QSet<QPair<double, double>>& result);
            void drawGraticuleIntersection (QPainter& p, const QPair<double, double>& g, const int& level);
            int drawGraticule (QPainter& p, const int& level);



        };
    }
}



#endif //CALENHAD_GRATICULE_H
