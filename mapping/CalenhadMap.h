//
// Created by martin on 10/08/17.
//

#ifndef CALENHAD_GLOBE_CALENHADMAP_H
#define CALENHAD_GLOBE_CALENHADMAP_H

#include <QOpenGLWindow>
#include <QtWidgets/QMainWindow>

namespace calenhad {
    namespace graph {
        class Graph;
    }
    namespace mapping {

        class CalenhadMapWidget;


        class CalenhadMap : public QMainWindow {
        public:
            CalenhadMap ();

            virtual ~CalenhadMap ();


            // void initializeGL() override;
            // void resizeGL (int width, int height) override;
            // void paintGL() override;

            void setGraph (calenhad::graph::Graph* g);

            CalenhadMapWidget* _widget;
        };
    }
}
#endif //CALENHAD_GLOBE_CALENHADMAP_H
