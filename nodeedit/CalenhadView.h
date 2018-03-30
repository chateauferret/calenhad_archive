//
// Created by martin on 27/10/16.
//

#ifndef CALENHAD_QNENODESVIEW_H
#define CALENHAD_QNENODESVIEW_H
#include <QGraphicsView>

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace nodeedit {
        class CalenhadController;

        class CalenhadView : public QGraphicsView {
        Q_OBJECT


        public:
            CalenhadView (QWidget* parent);

            ~CalenhadView ();

            void setController (CalenhadController* controller);

            double currentZoom ();

            void setZoom (const qreal& zoom);

            void dropEvent (QDropEvent* event) override;

            void dragMoveEvent (QDragMoveEvent* event) override;
            void dragEnterEvent (QDragEnterEvent* event) override;
            void dragLeaveEvent (QDragLeaveEvent* event) override;

        public slots:

        signals:

            void viewZoomed (double oldFactor, double newFactor);

        private:
            CalenhadController* _controller;
            int _steps = 0;
            qreal zoom = 1.0;
            QGraphicsItemGroup* g;


        };
    }
}


#endif //CALENHAD_QNENODESVIEW_H
