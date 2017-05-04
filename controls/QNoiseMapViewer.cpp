//
// Created by martin on 16/12/16.
//

#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsView>
#include "QNoiseMapViewer.h"
#include "../nodeedit/Calenhad.h"
#include "QNoiseMapExplorer.h"

using namespace geoutils;
using namespace Marble;


QNoiseMapViewer::QNoiseMapViewer (QModule* module, QWidget* parent) : CalenhadPreview (module, parent), _explorer (nullptr) {

}


QNoiseMapViewer::~QNoiseMapViewer() {
    if (_explorer) {
        delete _explorer;
    }
    if (_item) {
        delete _item;
    }
}

void QNoiseMapViewer::initialise() {
    CalenhadPreview::initialise();
    _scene = new QGraphicsScene();
    _scene -> installEventFilter (this);
    _view = new QGraphicsView (this);
    _view -> setScene (_scene);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    setLayout (new QVBoxLayout());
    layout() -> setMargin (5);
    _content = new QWidget();
    _view -> setFixedSize (250, 125);
    _contentLayout = new QVBoxLayout();
    _contentLayout -> setMargin (5);
    _content -> setLayout (_contentLayout);
    layout() -> addWidget (_content);
    layout() -> setSpacing (0);
    _contentLayout -> addWidget (_view);
    _label = new QLabel();
    _label -> setText ("Label");
    _contentLayout -> addWidget (_label);
    _progressBar = new QProgressBar (this);
    _contentLayout -> addWidget (_progressBar);
    connect (this, &CalenhadPreview::renderComplete, this, [=] () {
        if (_explorer) {
            _explorer -> changeView();
        }
    });

    render();
}

bool QNoiseMapViewer::eventFilter (QObject* o, QEvent* e) {
    QGraphicsSceneMouseEvent* me = (QGraphicsSceneMouseEvent*) e;
    switch ((int) e -> type ()) {
        case QEvent::GraphicsSceneMousePress:
            switch ((int) me -> button ()) {
                case Qt::LeftButton: {
                    if (_source -> isRenderable()) {
                        if (! _explorer) {
                            _explorer = new QNoiseMapExplorer (_source -> name(), _source);
                            connect (_explorer, SIGNAL (viewChanged (const GeoDataLatLonAltBox&)), this, SLOT (setBounds (const GeoDataLatLonAltBox&)));
                        }
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
/*
void QNoiseMapViewer::setProgress (int p) {
    _progressBar -> setValue (p);
}
*/
void QNoiseMapViewer::jobComplete (std::shared_ptr<QImage> image) {
    CalenhadPreview::jobComplete (image);
    QNoiseMapViewer::_label -> setText ("Rendered pixmap " + QDateTime::currentDateTime ().toString ());
    _scene -> clear();
    _item = _scene -> addPixmap (_pixmap);
    _scene -> setSceneRect (_item -> boundingRect());
    _item -> update();
}

void QNoiseMapViewer::setBounds (const Marble::GeoDataLatLonAltBox& bounds) {
    CalenhadPreview::setBounds (bounds);
}

QSize QNoiseMapViewer::renderSize () {
    return _view -> size();
}
