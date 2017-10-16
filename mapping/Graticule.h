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

            QList<double> graticules ();
            void drawGraticule (QPainter& p, const int& level = 0);

            bool visible () const;
            void setVisible (bool visible);
            int level () const;

            void setLevel (int level);
        protected:
            bool _visible;
            int _level;
            CalenhadMapWidget* _globe;
            void getIntersections (const QPair<double, double>& g, const double& interval, QSet<QPair<double, double>>& result);

            void drawGraticuleIntersection (QPainter& p, const QPair<double, double>& g, const int& level);


        };
    }
}



#endif //CALENHAD_GRATICULE_H
