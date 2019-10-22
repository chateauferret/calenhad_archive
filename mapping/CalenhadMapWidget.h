//
// Created by martin on 17/10/2019.
//

#ifndef MESSAGES_CALENHADMAPWIDGET_H
#define MESSAGES_CALENHADMAPWIDGET_H
#include "AbstractMapWidget.h"

namespace calenhad {
    namespace mapping {

        class CalenhadMapWidget : public AbstractMapWidget {
        public:
            CalenhadMapWidget (const controls::globe::RenderMode& mode = controls::globe::RenderMode::RenderModeGlobe, QWidget* parent = 0);
            ~CalenhadMapWidget ();

            void setGraph (calenhad::graph::Graph* g);
            calenhad::module::Module* source ();
            void setSource (calenhad::module::Module* qm);
            calenhad::legend::Legend* legend() override;
        public slots:
            void render ();

        protected:
            void initializeGL() override;
            void compute() override;
        };


    }
}

#endif //MESSAGES_CALENHADMAPWIDGET_H
