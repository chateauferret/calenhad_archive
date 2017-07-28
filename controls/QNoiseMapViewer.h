//
// Created by martin on 16/12/16.
//

#ifndef CALENHAD_QSPHERICALVIEWER_H
#define CALENHAD_QSPHERICALVIEWER_H


#include <QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QProgressBar>
#include <QGraphicsView>
#include <memory>
#include <marble/GeoDataLatLonAltBox.h>
#include "../geoutils.h"
#include "../icosphere/Bounds.h"
#include "../controls/globe/CalenhadPreview.h"


namespace calenhad {
    namespace qmodule {
        class QModule;
    }
    namespace controls {
        namespace globe {
            class QNoiseMapExplorer;
        }

        class QNoiseMapViewer : public calenhad::controls::globe::CalenhadPreview {
        Q_OBJECT
        public:
            QNoiseMapViewer (calenhad::qmodule::QModule* module, QWidget* parent);

            virtual ~QNoiseMapViewer ();

            void jobComplete (std::shared_ptr<QImage> image) override;

            void initialise ();

            bool eventFilter (QObject*, QEvent*);

            QSize renderSize () override;

        public slots:

            void setBounds (const icosphere::Bounds& bounds) override;
            // void setProgress (int p);

        protected:
            QLabel* _label;
            calenhad::controls::globe::QNoiseMapExplorer* _explorer;
            QProgressBar* _progressBar;
            QGraphicsView* _view;
            QGraphicsScene* _scene;
            QGraphicsPixmapItem* _item = nullptr;
            QWidget* _content;
            QVBoxLayout* _contentLayout;

        };
    }
}


#endif //CALENHAD_QSPHERICALVIEWER_H

