//
// Created by martin on 27/09/17.
//

#include "LegendEditorScale.h"
#include "LegendEditor.h"
#include "LegendEditorSlider.h"


using namespace calenhad::controls::legend;

LegendEditorScale::LegendEditorScale (QWidget* parent) : QWidget (parent) {
    setContentsMargins (0, 0, 0, 0);
}

void LegendEditorScale::paintEvent (QPaintEvent* e) {

    QPainter painter (this);

    QFont f = painter.font ();
    f.setPixelSize (8);
    painter.setFont (f);

    painter.setPen (Qt::black);
    painter.setBrush (_editor -> textColor_);

    QFontMetrics fm (f);

    // adjust size for vertical
    if (_editor->_orientation == Qt::Vertical) {
        QString txt1 = QString::number (_editor->_sliders.first() -> value(), 'f', _editor->textAcc_);
        QString txt2 = QString::number (_editor->_sliders.last() -> value(), 'f', _editor->textAcc_);
        int w = fm.width (txt1) + 4;
        if (w > this->width ()) { setFixedWidth (w); }
        w = fm.width (txt2) + 4;
        if (w > this->width ()) { setFixedWidth (w); }

        // draw the text for vertical orientation
        for (int i = 0; i < _editor->_sliders.size (); i++) {
            double pos = _editor -> _sliders [i] -> pos().y();
            qreal val = _editor -> _sliders[i] -> value();
            QString txt = QString::number (val, 'f', _editor->textAcc_);
            painter.drawText (2, (int) pos + _editor->_sliders[i]->height (), txt);
        }
    } else {
        for (int i = 0; i < _editor->_sliders.size (); i++) {
            double pos = _editor->_sliders [i] -> pos().x();
            qreal val = _editor->_sliders[i]-> value();
            QString txt = QString::number (val, 'f', _editor->textAcc_);
            if ((pos + fm.width (txt)) > width ()) { pos += -fm.width (txt) + _editor->_sliders[i]->width (); }
            painter.drawText ((int) pos, 2 + fm.height (), txt);
        }
    }

    QWidget::paintEvent (e);
}

void LegendEditorScale::setEditor (LegendEditor* editor) {
    _editor = editor;
}
