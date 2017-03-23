//
// Created by martin on 05/01/17.
//

#ifndef CALENHAD_QNOISEMAPEXPLORER_H
#define CALENHAD_QNOISEMAPEXPLORER_H
#include <libnoise/module/modulebase.h>
#include "../qmodule/QNode.h"
#include "../pipeline/TileProducer.h"
#include "CalenhadMarbleWidget.h"
#include "../mapping/CalenhadLayer.h"
#include "../libnoiseutils/GradientLegend.h"
#include <marble/MarbleWidget.h>
#include <QDialog>
#include <queue>
#include <marble/TileId.h>
#include <marble/GeoDataLatLonAltBox.h>
#include <QTimer>

class QColorGradient;
class TileProducer;
class QModule;
namespace Marble {
    class GeoDataLatLonAltBox;
}

using namespace Marble;



class QNoiseMapExplorer : public QDialog {
    Q_OBJECT

public:
    QNoiseMapExplorer (const QString& title, QModule* source, const QWidget* parent = 0);
    virtual ~QNoiseMapExplorer();

public slots:
    void changeView (const GeoDataLatLonBox&);
    void changeView (const GeoDataLatLonAltBox&);
    void changeBounds();
    void invalidate ();

    signals:
    void boundsChanged (const GeoDataLatLonBox&);

protected:
    QString _title, _name;
    CalenhadMarbleWidget* _mapWidget;
    QModule* _source;
    CalenhadLayer* _layer = nullptr;
    noise::utils::GradientLegend* _gradient;
    std::shared_ptr<QImage> _image;
    QTimer timer;
    GeoDataLatLonBox _bounds;
};

#endif //CALENHAD_QNOISEMAPEXPLORER_H
