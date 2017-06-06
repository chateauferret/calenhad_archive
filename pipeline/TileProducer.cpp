//
// Created by martin on 11/01/17.
//

#include <QQueue>
#include "TileProducer.h"
#include "../httpserver/CalenhadRequestHandler.h"
#include "../mapping/GeoSceneEquirectTileProjection.h"
#include <QThread>


using namespace geoutils;
using namespace Marble;

QCache<TileId, std::shared_ptr<QImage>>* TileProducer::_cache = new QCache<TileId, std::shared_ptr<QImage>>();

TileProducer::TileProducer (noise::module::Module* module, QString name) : _module (module), _name (name), _projection (new GeoSceneEquirectTileProjection()) {

}

TileProducer::~TileProducer() {
    delete _projection;

}

ImageRenderJob* TileProducer::makeRenderJob (TileId id, GeoSceneAbstractTileProjection* projection) {
   // ImageRenderJob* job = new ImageRenderJob (id, _node, projection);
   // return job;
    return nullptr;
}


std::shared_ptr<QImage> TileProducer::fetchTile (const int& x, const int& y, const int& z) {
    TileId id = TileId (_name, x, y, z);
    ImageRenderJob* job = makeRenderJob (id, _projection);
    std::shared_ptr<QImage> image = std::make_shared<QImage> (_size, _size, QImage::Format_ARGB32);
    image -> fill (QColor (0, 0, 255));
    job -> setImage (image);
    job -> render();
    emit tileProduced (id, image);
    return job -> image();
}

std::shared_ptr<QImage> TileProducer::fetchTile (const TileId& id) {
    // tile is a bare pointer to a shared pointer. If the cache deletes the bare pointer the shared pointer still provides access to the pixmap.
    // try and find the requested tile in the cache
    //tile = _cache -> object (id);
    //if (! tile) {
    std::cout << "Generating tile " << id.x() << ", " << id.y() << " zoom level " << id.zoomLevel () <<"\n";
    std::shared_ptr<QImage> image = std::make_shared<QImage> (_size, _size, QImage::Format_ARGB32);
    image -> fill (QColor (0, 0, 255));
        // if tile was not found, make it and save it in the cache
    ImageRenderJob* job = makeRenderJob (id, _projection);

    QThread* thread = new QThread();

    job -> setImage (image);
    job -> moveToThread (thread);
    connect (thread, SIGNAL (started()), job, SLOT (startJob()));
    connect (job, SIGNAL (complete (TileId, std::shared_ptr<QImage>)), this, SLOT (tileGenerated (TileId, std::shared_ptr<QImage>)));
    connect (job, SIGNAL (complete (TileId, std::shared_ptr<QImage>)), thread, SLOT (quit ()));
    connect (thread, SIGNAL (finished ()), thread, SLOT (deleteLater ()));
    connect (job, SIGNAL (complete (TileId, std::shared_ptr<QImage>)), job, SLOT (deleteLater()));
    thread -> start ();
    //    _cache -> insert (id, tile), 1;
    //} else {
    //    std::cout << "Found tile in cache: " << id.x() << ", " << id.y() << " zoom level " << id.zoomLevel () << "\n";
    //}
    //std::cout << "emit signal\n";
    //std::shared_ptr<QImage> pixmap = *tile;


    //std::cout << " ... complete\n";

    return image;
}

void TileProducer::tileGenerated (TileId id, std::shared_ptr<QImage> image) {
    std::cout << " Tile generated \n";
    emit tileProduced (id, image);
}

