//
// Created by martin on 17/10/2019.
//

#ifndef MESSAGES_CALENHADMAPWIDGET_H
#define MESSAGES_CALENHADMAPWIDGET_H
#include "AbstractMapWidget.h"

namespace calenhad {
    namespace grid {
        class Icosphere;
    }
    namespace mapping {

        class CalenhadMapWidget : public AbstractMapWidget {
        public:
            explicit CalenhadMapWidget (const controls::globe::RenderMode& mode = controls::globe::RenderMode::RenderModeGlobe, QWidget* parent = 0);
            ~CalenhadMapWidget () override;

            calenhad::module::Module* source ();
            void setSource (calenhad::module::Module* qm);
            calenhad::legend::Legend* legend() override;
            //void fillIcosphere (grid::Icosphere* icosphere);

            void compute() override;

        public slots:
            void render () override;
            void showEvent (QShowEvent* event) override;

        protected:
            void initializeGL() override;

            bool _computeCubeMap;
            GLfloat* _cubeMapBuffer{};
            GLfloat* _gridBuffer{};


            void updateParams ();
        };


    }
}

#endif //MESSAGES_CALENHADMAPWIDGET_H
