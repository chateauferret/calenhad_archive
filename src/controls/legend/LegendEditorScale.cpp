//
// Created by martin on 27/09/17.
//

#include "LegendEditorScale.h"
#include "LegendEditor.h"
#include "LegendEditorSlider.h"

#include "LegendEditorSliderPanel.h"


using namespace calenhad::controls::legend;

LegendEditorScale::LegendEditorScale (LegendEditorSliderPanel* parent) : QWidget (parent), _panel (parent) {
    setContentsMargins (0, 0, 0, 0);
}

void LegendEditorScale::paintEvent (QPaintEvent* e) {

    QPainter painter (this);

    QFont f = painter.font ();
    f.setPixelSize (8);
    painter.setFont (f);

    painter.setPen (Qt::black);
    painter.setBrush (Qt::black);

    QFontMetrics fm (f);

    // adjust size for vertical
    if (_panel -> orientation() == Qt::Vertical) {
        QString txt1 = QString::number (_panel -> _sliders.first() -> value(), 'f', 2);
        QString txt2 = QString::number (_panel -> _sliders.last() -> value(), 'f', 2);
        int w = fm.horizontalAdvance (txt1) + 4;
        if (w > this->width ()) { setFixedWidth (w); }
        w = fm.horizontalAdvance (txt2) + 4;
        if (w > this->width ()) { setFixedWidth (w); }

        // draw the text for vertical orientation
        for (int i = 0; i < _panel -> _sliders.size (); i++) {
            double pos = _panel -> _sliders [i] -> pos().y();
            qreal val = _panel -> _sliders [i] -> value();
            QString txt = QString::number (val, 'f', 2);
            painter.drawText (2, (int) pos + _panel -> _sliders [i] -> height(), txt);
        }
    } else {
        for (int i = 0; i < _panel -> _sliders.size (); i++) {
            double pos = _panel ->_sliders [i] -> pos().x();
            qreal val = _panel ->_sliders [i]-> value();
            QString txt = QString::number (val, 'f', 2);
            if ((pos + fm.horizontalAdvance (txt)) > width ()) { pos += -fm.width (txt) + _panel -> _sliders [i]->width (); }
            painter.drawText ((int) pos, 2 + fm.height (), txt);
        }
    }

    QWidget::paintEvent (e);
}
