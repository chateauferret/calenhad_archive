//
// Created by martin on 05/01/17.
//

#ifndef CALENHAD_QNOISEMAPEXPLORER_H
#define CALENHAD_QNOISEMAPEXPLORER_H
#include <libnoise/module/modulebase.h>
#include "../icosphere/Bounds.h"
#include <marble/MarbleWidget.h>
#include <QDialog>
#include <queue>
#include <marble/TileId.h>
#include <marble/GeoDataLatLonAltBox.h>
#include <QTimer>
#include <memory>

class QColorGradient;

namespace Marble {
    class GeoDataLatLonAltBox;
}

namespace calenhad {
    namespace qmodule {
        class QModule;
    }
    namespace legend {
        class Legend;
    }
    namespace controls {
        namespace globe {
            class CalenhadGlobe;
            class CalenhadOverviewMap;



            class QNoiseMapExplorer : public QDialog {
            Q_OBJECT

            public:
                QNoiseMapExplorer (const QString& title, calenhad::qmodule::QModule* source, const QWidget* parent = 0);

                virtual ~QNoiseMapExplorer ();

            public slots:

                void changeView ();

            signals:

                void viewChanged (const icosphere::Bounds&);

            protected:
                QString _title, _name;
                CalenhadGlobe* _mapWidget;
                calenhad::qmodule::QModule* _source;

                calenhad::legend::Legend* _gradient;
                std::shared_ptr<QImage> _image;

            protected slots:

                void updateView (const icosphere::Bounds&);


            };
        }
    }
}

#endif //CALENHAD_QNOISEMAPEXPLORER_H
