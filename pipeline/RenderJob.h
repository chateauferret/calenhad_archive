//
// Created by martin on 30/12/16.
//

#ifndef CALENHAD_RENDERJOB_H
#define CALENHAD_RENDERJOB_H


#include <QtGui/QImage>
#include <QtCore/QRunnable>
#include <QtCore/QMutex>
#include "../geoutils.h"

#include <memory>
#include <marble/GeoDataLatLonBox.h>

namespace noise {
    namespace utils {
        class RendererImage;
        class Image;
    }
    namespace module {
        class Module;
    }
}

enum RenderJobStatus { Pending, Underway, Paused, Complete, Invalid, Cancelled };
Q_DECLARE_METATYPE (RenderJobStatus);


class RenderJob : public QObject {
Q_OBJECT
public:
    RenderJob (const Marble::GeoDataLatLonBox& bounds, noise::module::Module* source);
    virtual ~RenderJob ();
    void setImage (std::shared_ptr<QImage>& image);
    bool canRender ();
    void setSource (noise::module::Module* source);
    RenderJobStatus status ();
    void render ();

public slots:
    void cancel();
    void startJobAsync ();
    void sendProgress (int);
signals:
    void progress (int);
    void complete();
    void status (RenderJobStatus);

protected:
    Marble::GeoDataLatLonBox _bounds;
    // the image is referenced by a shared pointer because we don't know what will be destructed when
    std::shared_ptr<QImage> _image = nullptr;
    noise::module::Module* _source;
    RenderJobStatus _status;
    void setStatus (RenderJobStatus s);
    int _percentComplete;
    static bool declared ;
    QImage fetchImage (int height, int width);
};

#endif //CALENHAD_RENDERJOB_H
