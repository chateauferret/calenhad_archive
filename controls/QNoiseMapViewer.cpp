//
// Created by martin on 16/12/16.
//

#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsView>
#include "QNoiseMapViewer.h"
#include "../nodeedit/qnemainwindow.h"
#include "QNoiseMapExplorer.h"
#include "../qmodule/QModule.h"


using namespace geoutils;

QNoiseMapViewer::QNoiseMapViewer (QModule* parent) : QWidget (parent),
    _isRendered (false),
    _bounds (Geolocation (-180.0, -90.0, Geolocation::DEGS), Geolocation (180.0, 90.0, Geolocation::DEGS)),
    _source (nullptr), _explorer (nullptr) {
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
}

QNoiseMapViewer::~QNoiseMapViewer() {

    if (_item) {
        delete _item;
    }
    if (_job) {
        delete _job;
    }
    if (_explorer) {
        delete _explorer;
    }
}

void QNoiseMapViewer::setBounds (const GeoQuad& bounds) {
    _bounds = bounds;
}

GeoQuad QNoiseMapViewer::bounds () {
    return _bounds;
}

void QNoiseMapViewer::setSize (const int& height, const int& width) {
    _view -> setFixedHeight (height);
    _view -> setFixedWidth (width);
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
    render ();
}

void QNoiseMapViewer::render() {
    if (_job) {
        delete _job;
    }
    QModule* module = (QModule*) parent ();
        if (module -> isEnabled ()) {
            _job = new RenderJob (_bounds, _source->module ());

            QThread* thread = new QThread ();

            _image = std::make_shared<QImage> (2 * height (), height (), QImage::Format_ARGB32);
            _progressBar->setMinimum (0);
            _progressBar->setMaximum (height ());
            _progressBar->setValue (0);
            _job->setImage (_image);
            _job->moveToThread (thread);
            connect (thread, SIGNAL (started ()), _job, SLOT (startJobAsync ()));
            connect (_job, SIGNAL (complete ()), this, SLOT (jobComplete ()));
            connect (_job, SIGNAL (complete ()), thread, SLOT (quit ()));
            connect (_job, SIGNAL (progress (int)), this, SLOT (setProgress (int)));
            connect (thread, SIGNAL (finished ()), thread, SLOT (deleteLater ()));
            thread->start ();
            _label->setText (_bounds.toString ());
        }
}

void QNoiseMapViewer::jobComplete() {
    QPixmap pixmap = QPixmap::fromImage (*_image);
    _item = _scene -> addPixmap (pixmap);
    _scene -> setSceneRect (_item -> boundingRect());
    _isRendered = true;

}

void QNoiseMapViewer::jobUpdate (RenderJobStatus status) {


    if (status == RenderJobStatus::Pending) {
        // to do - progress UI
    }
    if (status == RenderJobStatus::Underway) {

    }
}

std::shared_ptr<QImage> QNoiseMapViewer::image () {
    return _image;
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

                }

                case Qt::RightButton: {
                    if (_source -> isRenderable ()) {
                        if (!_explorer) {
                            _explorer = new QNoiseMapExplorer (_source->name ());
                        }

                        _explorer->setSource (_source->module ());
                        _explorer->show ();
                    }
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
