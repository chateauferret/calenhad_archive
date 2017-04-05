//
// Created by martin on 16/12/16.
//

#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsView>
#include "QNoiseMapViewer.h"
#include "../nodeedit/Calenhad.h"
#include "QNoiseMapExplorer.h"
#include "../qmodule/QModule.h"


using namespace geoutils;
using namespace Marble;

QNoiseMapViewer::QNoiseMapViewer (QModule* parent) : QWidget (parent),
    _isRendered (false),
    _bounds (GeoDataLatLonBox (-M_PI_2, M_PI_2, -M_PI, M_PI)),
    _source (nullptr), _image (nullptr), _explorer (nullptr), _previewType (NoiseMapPreviewType::WholeWorld) {
}

void QNoiseMapViewer::initialise() {
    _scene = new QGraphicsScene();
    _view = new QGraphicsView (this);
    _view -> setScene (_scene);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    layout() -> setContentsMargins (0, 0, 0, 0);
    layout() -> setMargin (0);
    _content = new QWidget();
    _layout = new QVBoxLayout();
    _layout -> setContentsMargins (0, 0, 0, 0);
    _content -> setLayout (_layout);
    layout() -> addWidget (_content);
    layout() -> setSpacing (0);
    _layout -> addWidget (_view);
    _label = new QLabel();
    _label -> setText ("Label");
    _layout -> addWidget (_label);
    _scene -> installEventFilter (this);
    _progressBar = new QProgressBar (this);
    _layout -> addWidget (_progressBar);
    setSize (105);

}

QNoiseMapViewer::~QNoiseMapViewer() {

    if (_item) {
        delete _item;
    }

    if (_explorer) {
        delete _explorer;
    }
}

void QNoiseMapViewer::setBounds (const GeoDataLatLonBox& bounds) {
    if (bounds != _bounds) {
        _bounds = bounds;
        if (_previewType == NoiseMapPreviewType::ExplorerBounds) {
            render ();
        }
    }

}

GeoDataLatLonBox QNoiseMapViewer::bounds () {
    return _bounds;
}

void QNoiseMapViewer::setSize (const int& height) {
    _view -> setFixedHeight (height);
}

int QNoiseMapViewer::height () {
    return _view -> height();
}

int QNoiseMapViewer::width () {
    return _view -> width();
}

bool QNoiseMapViewer::isRendered () {
    return _isRendered;
}

void QNoiseMapViewer::showEvent (QShowEvent *) {

}

void QNoiseMapViewer::render() {

    QModule* module = (QModule*) parent ();
        if (module -> isInitialised ()) {
            RenderJob* job = new RenderJob (_bounds, _source -> module ());
            QThread* thread = new QThread();
            int width = _previewType == NoiseMapPreviewType::WholeWorld ? 2 * height() : height();
            std::shared_ptr<QImage> image = std::make_shared<QImage> (width, height(), QImage::Format_ARGB32);
            job -> setImage (image);
            _image = image;
            job -> moveToThread (thread);
            connect (thread, SIGNAL (started ()), job, SLOT (startJob()));
            connect (job, SIGNAL (complete (TileId, std::shared_ptr<QImage>)), this, SLOT (jobComplete (TileId, std::shared_ptr<QImage>)));
            connect (job, SIGNAL (complete (TileId, std::shared_ptr<QImage>)), thread, SLOT (quit ()));
            connect (job, SIGNAL (progress (int)), this, SLOT (setProgress (int)));
            connect (thread, SIGNAL (finished ()), thread, SLOT (deleteLater ()));

            thread -> start ();
            _label -> setText (_bounds.toString ());
       }
}

void QNoiseMapViewer::jobComplete (TileId, std::shared_ptr<QImage> image) {
    std::cout << "Rendered\n";
    _pixmap = QPixmap::fromImage (image->mirrored ());
    _scene -> clear();
    _item = _scene -> addPixmap (_pixmap);
    _scene -> setSceneRect (_item -> boundingRect());
    _isRendered = true;
    if (_explorer) {
        _explorer -> changeView (_bounds);
    }
    emit renderComplete();
    _label -> setText ("Rendered image " + QDateTime::currentDateTime ().toString ());
    _item -> update();
}

QModule* QNoiseMapViewer::source() {
    return _source;
}

void QNoiseMapViewer::setSource (QModule* qm) {
    _source = qm;
}

bool QNoiseMapViewer::eventFilter (QObject* o, QEvent* e) {
    QGraphicsSceneMouseEvent* me = (QGraphicsSceneMouseEvent*) e;
    switch ((int) e -> type ()) {
        case QEvent::GraphicsSceneMousePress:
            switch ((int) me -> button ()) {
                case Qt::LeftButton: {
                    if (_source -> isRenderable()) {
                        if (_explorer) {
                            _explorer -> close ();
                            delete _explorer;
                        }
                        _explorer = new QNoiseMapExplorer (_source -> name(), _source);
                        _explorer -> show ();
                    }
                    break;
                }

                case Qt::RightButton: {

                }
            }

        case QEvent::GraphicsSceneMouseMove: {


        }

        //case QEvent::GraphicsSceneMouseRelease:

    }
    return QObject::eventFilter (o, e);
}

void QNoiseMapViewer::setProgress (int p) {
    _progressBar -> setValue (p);
}
