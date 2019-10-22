//
// Created by martin on 10/10/17.
//

#include <qwt/qwt_scale_engine.h>
#include "GlobeScaleWidget.h"
#include "mapping/AbstractMapWidget.h"
#include <QPainter>
#include <geoutils.h>


using namespace calenhad::controls::globe;
using namespace geoutils;
using namespace calenhad::mapping;
using namespace calenhad::mapping::projection;

GlobeScaleWidget::GlobeScaleWidget (AbstractMapWidget* globe, QWidget* parent, const double& radius) : QwtScaleWidget (parent), _orientation (Qt::Horizontal), _globe (globe), _radius (radius), _metresPerUnit (1609) {
    double s;
    _geodesic = new GeographicLib::Geodesic (radius, 0);
    _draw = new QwtScaleDraw();
    setScaleDraw (_draw);
    _scaleEngine = new QwtLinearScaleEngine();
    _draw -> setAlignment (QwtScaleDraw::Alignment::TopScale);
}

GlobeScaleWidget::~GlobeScaleWidget() {
    //if (_scaleEngine) { delete _scaleEngine; }
    //if (_draw) { delete _draw; }
    delete _geodesic;
}

void GlobeScaleWidget::paintEvent (QPaintEvent* e) {
    QPainter painter (this);

    int extent = (_orientation == Qt::Horizontal ? width() : height()) - 10;
    QPointF centre (_globe -> width() / 2, _globe -> height() / 2);
    QPointF p1 = _orientation == Qt::Horizontal ? QPointF (centre.x() - extent / 2, centre.y()) : QPointF (centre.x(), centre.y() - extent / 2);
    Geolocation a;
    bool visible = _globe -> geoCoordinates (p1, a);
    if (visible) {
        Geolocation b;
        QPointF p2 = _orientation == Qt::Horizontal ? QPointF (centre.x() + extent / 2, centre.y()) : QPointF (centre.x(), centre.y() + extent / 2);
        visible = _globe -> geoCoordinates (p2, b);
        double upperBound;
        double s = _geodesic -> Inverse (a.latitude (Units::Degrees), a.longitude (Units::Degrees), b.latitude (Units::Degrees), b.longitude (Units::Degrees), upperBound);
        QwtScaleDiv div = _scaleEngine -> divideScale (0, upperBound / _metresPerUnit, 5, 5, 0.0);
        _draw -> setScaleDiv (div);
    }

    QBrush b = QBrush();
    b.setColor (CalenhadServices::preferences() -> calenhad_globe_scale_background_color);
    b.setStyle (Qt::BrushStyle::SolidPattern);
    painter.setBrush (b);
    painter.drawRect (0, 0, width() - 10, height());

    QwtScaleWidget::paintEvent (e);
}

void GlobeScaleWidget::setOrientation (Qt::Orientation orientation) {
    _orientation = orientation;
    update();
}
