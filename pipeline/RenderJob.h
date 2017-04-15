//
// Created by martin on 30/12/16.
//

#ifndef CALENHAD_RENDERJOB_H
#define CALENHAD_RENDERJOB_H


#include <QtGui/QImage>
#include <QtCore/QRunnable>
#include <QtCore/QMutex>
#include "../geoutils.h"
#include <QMutex>
#include <memory>
#include <marble/GeoDataLatLonBox.h>
#include <marble/TileId.h>


namespace noise {
    namespace utils {
        class RendererImage;
        class Image;
    }
    namespace module {
        class Module;
    }
}

namespace Marble {
    class GeoSceneAbstractTileProjection;
}

namespace icosphere {
    class Legend;
}

using namespace Marble;

class RenderJob : public QObject {
Q_OBJECT
public:
    static constexpr int MAX_ZOOM = 24;
    RenderJob (const TileId& id, noise::module::Module* source, GeoSceneAbstractTileProjection* projection);
    RenderJob (const GeoDataLatLonBox& box, noise::module::Module* source, icosphere::Legend* legend);
    virtual ~RenderJob ();
    void setImage (std::shared_ptr<QImage>& image);
    bool canRender ();
    void render ();
    std::shared_ptr<QImage> image ();
    bool isAbandoned();

public slots:
    void startJob ();
    void sendProgress (int);
    void abandon ();

signals:
    void progress (int);
    void complete (TileId, std::shared_ptr<QImage>);

protected:
    Marble::GeoDataLatLonBox _bounds;
    // the image is referenced by a shared pointer because we don't know what will be destructed when
    std::shared_ptr<QImage> _image = nullptr;
    noise::module::Module* _source;
    int _percentComplete;
    Marble::TileId _id;
    bool _abandoned = false;
    QMutex _mutex;
    icosphere::Legend* _legend;

};

#endif //CALENHAD_RENDERJOB_H
