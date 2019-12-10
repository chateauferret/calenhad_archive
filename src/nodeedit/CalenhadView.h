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
            explicit CalenhadView (QWidget* parent);

            ~CalenhadView() override;

            void setController (CalenhadController* controller);

            double currentZoom ();

            void setZoom (const qreal& zoom);
            void dropEvent (QDropEvent* event) override;

            void dragMoveEvent (QDragMoveEvent* event) override;
            void dragEnterEvent (QDragEnterEvent* event) override;

            void setGridVisible (const bool& visible);
            bool gridVisible();
            void setSnapToGrid (const bool& enabled);
            bool snapToGrid();
            void setModel (pipeline::CalenhadModel* model);

        public slots:
            void modelChanged();
        signals:
            void zoomInRequested();
            void zoomOutRequested();
            void viewZoomed (double oldFactor, double newFactor);

        protected:
            CalenhadController* _controller;
            int _steps = 0;
            qreal zoom = 1.0;

            void wheelEvent (QWheelEvent* event) override;
            void drawBackground (QPainter *painter, const QRectF &rect) override;


        };
    }
}


#endif //CALENHAD_QNENODESVIEW_H
