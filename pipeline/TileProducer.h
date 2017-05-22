//
// Created by martin on 11/01/17.
//

#ifndef CALENHAD_TILEPRODUCER_H
#define CALENHAD_TILEPRODUCER_H


#include <QtGui/QImage>
#include <memory>
#include <marble/GeoDataLatLonBox.h>
#include <marble/TileId.h>
#include <QtCore/QCache>
#include <QQueue>
#include <marble/ViewportParams.h>
#include "CalenhadModel.h"

namespace geoutils {
    class Geolocation;
}

namespace noise {
    namespace module {
        class Module;
    }
}

namespace Marble {
    class GeoSceneAbstractTileProjection;
    class GeoSceneEquirectTileProjection;
}

using namespace Marble;

class ImageRenderJob;

class TileProducer : public QObject {
Q_OBJECT


public:
    TileProducer (noise::module::Module* module, QString name);
    virtual ~TileProducer();

    static constexpr unsigned DEFAULT_SIZE = 256;
    int _zoom = 1, _size = DEFAULT_SIZE;
    static int powers (int index);
    std::shared_ptr<QImage> fetchTile (const int& x, const int& y, const int& z);
    std::shared_ptr<QImage> fetchTile (const Marble::TileId& id);

public slots:
    void tileGenerated (TileId, std::shared_ptr<QImage>);

signals:
    void tileProduced (TileId, std::shared_ptr<QImage>);

protected:
    noise::module::Module* _module;
    QString _name;
    Marble::GeoSceneEquirectTileProjection* _projection;
    static QCache<Marble::TileId, std::shared_ptr<QImage>>* _cache;
    ImageRenderJob* makeRenderJob (TileId id, GeoSceneAbstractTileProjection* projection);
};




#endif //CALENHAD_TILEPRODUCER_H
