//
// Created by martin on 27/10/16.
//

#ifndef CALENHAD_QNENODESVIEW_H
#define CALENHAD_QNENODESVIEW_H
#include <QGraphicsView>
#include "Calenhad.h"

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace nodeedit {
        class CalenhadController;

        class CalenhadView : public QGraphicsView {
        Q_OBJECT


        public:
            explicit CalenhadView (calenhad::nodeedit::Calenhad* parent);

            ~CalenhadView() override;

            double currentZoom ();

            void setModel (pipeline::CalenhadModel* model);

            void setZoom (const qreal& zoom);
            void dropEvent (QDropEvent* event) override;

            void dragMoveEvent (QDragMoveEvent* event) override;
            void dragEnterEvent (QDragEnterEvent* event) override;

            void setGridVisible (const bool& visible);
            bool gridVisible();
            void setSnapToGrid (const bool& enabled);
            bool snapToGrid();

        public slots:
            void modelChanged();
        signals:
            void zoomInRequested();
            void zoomOutRequested();
            void viewZoomed (double oldFactor, double newFactor);

        protected:
            int _steps = 0;
            qreal zoom = 1.0;

            void wheelEvent (QWheelEvent* event) override;
            void drawBackground (QPainter *painter, const QRectF &rect) override;

        };
    }
}


#endif //CALENHAD_QNENODESVIEW_H
