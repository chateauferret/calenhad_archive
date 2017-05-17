//
// Created by martin on 02/05/17.
//

#include "QNoiseMapExplorer.h"
#include "../nodeedit/Calenhad.h"
#include "QNoiseMapViewer.h"
#include <QThread>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include "CalenhadPreview.h"

CalenhadPreview::CalenhadPreview (QModule* module, QWidget* parent) :
        QWidget (parent),
        _isRendered (false), _ratio (2.0),
        _bounds (GeoDataLatLonBox (-M_PI_2, M_PI_2, 0, M_2_PI)),
        _source (module), _image (nullptr), _previewType (OverviewPreviewType::WholeWorld) {
}

CalenhadPreview::~CalenhadPreview() {

}

void CalenhadPreview::setBounds (const GeoDataLatLonAltBox& bounds) {
    if (bounds != _bounds) {
        _bounds = bounds;
        _previewType = OverviewPreviewType::ExplorerBounds;
        _ratio = bounds.width() / bounds.height();
        render ();
    }
}

GeoDataLatLonBox CalenhadPreview::bounds () {
    return _bounds;
}

bool CalenhadPreview::isRendered () {
    return _isRendered;
}

void CalenhadPreview::showEvent (QShowEvent *) {

}

void CalenhadPreview::render() {
        if (_source -> legend() && height() > 0) {
            RenderJob* job = prepareRender();
            QThread* thread = new QThread();
            job -> moveToThread (thread);
            connect (thread, SIGNAL (started ()), job, SLOT (startJob()));
            connect (job, SIGNAL (complete (std::shared_ptr<QImage>)), this, SLOT (jobComplete (std::shared_ptr<QImage>)));
            connect (job, SIGNAL (complete (std::shared_ptr<QImage>)), thread, SLOT (quit ()));
            //connect (job, SIGNAL (progress (int)), this, SLOT (QNoiseMapViewer::setProgress (int)));
            connect (thread, SIGNAL (finished ()), thread, SLOT (deleteLater ()));
            thread -> start ();
       }
}

RenderJob* CalenhadPreview::prepareRender() {
    RenderJob* job = new RenderJob (_bounds, _source -> module(), _source -> legend());
    int height = renderSize().height();
    int width = (int) (renderSize().height() * _ratio);
    std::shared_ptr<QImage> image = std::make_shared<QImage> (width, height, QImage::Format_ARGB32);
    job -> setImage (image);
    _image = image;
    return job;
}

void CalenhadPreview::jobComplete (std::shared_ptr<QImage> image) {
    if (_previewType == OverviewPreviewType::ExplorerBounds) {
        *image = image -> mirrored (false, true);
    }
    _pixmap = QPixmap::fromImage (*image);
    _isRendered = true;
    emit renderComplete (image);

}

QModule* CalenhadPreview::source() {
    return _source;
}

void CalenhadPreview::setSource (QModule* qm) {
    _source = qm;
}

QSize CalenhadPreview::renderSize() {
    return QSize ((int) (height() * _ratio), height());
}

void CalenhadPreview::initialise () {

}

std::shared_ptr<QImage> CalenhadPreview::image() {
    return _image;
}
