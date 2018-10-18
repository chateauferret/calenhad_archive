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
            void setGridVisible (const bool& visible);
            bool gridVisible();


        public slots:

        signals:
            void zoomInRequested();
            void zoomOutRequested();
            void viewZoomed (double oldFactor, double newFactor);

        protected:
            CalenhadController* _controller;
            int _steps = 0;
            qreal zoom = 1.0;
            QGraphicsItemGroup* g;
            void wheelEvent (QWheelEvent* event) override;
            void drawBackground (QPainter *painter, const QRectF &rect) override;
            bool _gridVisible;

        };
    }
}


#endif //CALENHAD_QNENODESVIEW_H
