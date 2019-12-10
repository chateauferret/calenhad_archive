//
// Created by martin on 27/09/17.
//

#include <QtWidgets/QWidget>
#include "LegendPreview.h"
#include "LegendEditor.h"
#include <qwt/qwt_color_map.h>
#include <qwt/qwt_transform.h>
#include "../../legend/Legend.h"


using namespace calenhad::legend;
using namespace calenhad::controls::legend;

LegendPreview::LegendPreview (QWidget* parent) : QwtScaleWidget (parent) {
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    setContentsMargins (0, 0, 0, 0);
    setFixedHeight (96);
    setMinimumWidth (24);

    _draw = new QwtScaleDraw();
    setScaleDraw (_draw);
    _scaleEngine = new QwtLinearScaleEngine();
    _draw -> setAlignment (QwtScaleDraw::Alignment::TopScale);
    _draw -> setSpacing (0.0);
}

LegendPreview::~LegendPreview() {

}

void LegendPreview::paintEvent (QPaintEvent* e) {
    QPainter painter (this);
    painter.setPen (Qt::black);
    QColor color;
    double lowerBound = _editor -> pan() - _editor -> zoom();
    double upperBound = _editor -> pan() + _editor -> zoom();
    QwtScaleDiv div = _scaleEngine -> divideScale (lowerBound, upperBound, 5, 5, 0.0);
    _draw -> setScaleDiv (div);

    for (int i = 0; i < width(); i++) {
    double index = _editor -> valueAt (i);
        color = _editor -> _legend -> lookup (index);
        QPen pen = QPen (color);
        painter.setPen (pen);
        if (_editor->_orientation == Qt::Horizontal) {
            painter.drawLine (i, _draw -> minimumExtent (), i, height() - _draw -> minimumExtent ());
        } else {
            painter.drawLine (_draw -> minimumExtent (), i, width() - _draw -> minimumExtent (), i);
        }
    }

    QwtScaleWidget::paintEvent (e);
}
