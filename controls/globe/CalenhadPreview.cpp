//
// Created by martin on 02/05/17.
//

#include "QNoiseMapExplorer.h"
#include "nodeedit/Calenhad.h"
#include "controls/QNoiseMapViewer.h"
#include <QThread>
#include "../../qmodule/QModule.h"
#include "../../legend/Legend.h"
#include "../../pipeline/ImageRenderJob.h"

using namespace icosphere;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;


CalenhadPreview::CalenhadPreview (QModule* module, QWidget* parent) :
        QWidget (parent),
        _isRendered (false), _ratio (2.0),
        _bounds (Bounds (-M_PI_2, M_PI_2, 0, M_2_PI)),
        _source (module), _image (nullptr), _previewType (OverviewPreviewType::WholeWorld) {
}

CalenhadPreview::~CalenhadPreview() {

}

void CalenhadPreview::setBounds (const Bounds& bounds) {
    if (bounds != _bounds) {
        _bounds = bounds;
        _previewType = OverviewPreviewType::ExplorerBounds;
        _ratio = bounds.width() / bounds.height();
        render ();
    }
}

Bounds CalenhadPreview::bounds () {
    return _bounds;
}

bool CalenhadPreview::isRendered () {
    return _isRendered;
}

void CalenhadPreview::showEvent (QShowEvent *) {

}

void CalenhadPreview::render() {
        if (_source -> legend() && height() > 0 && _source -> isEnabled()) {
            ImageRenderJob* job = prepareRender();
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

ImageRenderJob* CalenhadPreview::prepareRender() {
    ImageRenderJob* job = new ImageRenderJob (_bounds, _source, _source -> legend());
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
