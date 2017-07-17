//
// Created by martin on 05/01/17.
//

#ifndef CALENHAD_QNOISEMAPEXPLORER_H
#define CALENHAD_QNOISEMAPEXPLORER_H
#include <libnoise/module/modulebase.h>
#include "../qmodule/QNode.h"
#include "../pipeline/TileProducer.h"
#include "CalenhadGlobe.h"
#include "../mapping/CalenhadLayer.h"
#include "../mapping/Legend.h"
#include "../icosphere/Bounds.h"
#include <marble/MarbleWidget.h>
#include <QDialog>
#include <queue>
#include <marble/TileId.h>
#include <marble/GeoDataLatLonAltBox.h>
#include <QTimer>

class QColorGradient;
class TileProducer;
class QModule;
class CalenhadOverviewMap;
namespace Marble {
    class GeoDataLatLonAltBox;
}

using namespace Marble;
using namespace icosphere;


class QNoiseMapExplorer : public QDialog {
    Q_OBJECT

public:
    QNoiseMapExplorer (const QString& title, QModule* source, const QWidget* parent = 0);
    virtual ~QNoiseMapExplorer();

    public slots:
    void changeView();

    signals:
    void viewChanged (const Bounds&);

protected:
    QString _title, _name;
    CalenhadGlobe* _mapWidget;
    QModule* _source;

    Legend* _gradient;
    std::shared_ptr<QImage> _image;

    protected slots:
    void updateView (const Bounds&);


};

#endif //CALENHAD_QNOISEMAPEXPLORER_H
