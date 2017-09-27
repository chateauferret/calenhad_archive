/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** QColorRampEditor is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/

#include "LegendEditor.h"
#include <QMouseEvent>
#include <cmath>
#include "CalenhadServices.h"
#include "legend/Legend.h"

using namespace calenhad::legend;

// -----------------------------------------------------------
// LegendEditor ------------------------------------------
// -----------------------------------------------------------
LegendEditor::LegendEditor (Legend* legend, QWidget* parent, int orientation) : QWidget (parent), _legend (legend),
                                                                        _orientation (orientation),
                                                                        activeSlider_ (-1), slideUpdate_ (false),
                                                                        bspace_ (5), _zoom (1.0), _pan (0.0),
                                                                        visText_ (false), textColor_ (Qt::white), textAcc_ (1) {
    if (_orientation == Qt::Horizontal) {
        setMinimumSize (50, 40);
    } else {
        setMinimumSize (40, 50);
    }

    setContentsMargins (0, 0, 0, 0);
    if (_orientation == Qt::Horizontal) {
        setLayout (new QVBoxLayout ());
    } else {
        setLayout (new QHBoxLayout ());
    }
    layout ()->setMargin (0);
    layout ()->setSpacing (0);
    layout ()->setContentsMargins (0, 0, 0, 0);

    rampwid_ = new QRampWidget ();
    rampwid_->_editor = this;
    rampwid_->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    rampwid_->setContentsMargins (0, 0, 0, 0);

    layout ()->addWidget (rampwid_);

    slidewid_ = new QSlidersWidget ();
    slidewid_->_editor = this;
    if (_orientation == Qt::Horizontal) {
        slidewid_->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
        slidewid_->setFixedHeight (16);
    } else {
        slidewid_->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
        slidewid_->setFixedWidth (16);
    }
    slidewid_->setContentsMargins (0, 0, 0, 0);
    layout ()->addWidget (slidewid_);

    textwid_ = new QSliderTextWidget ();
    textwid_->_editor = this;
    if (_orientation == Qt::Horizontal) {
        textwid_->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
        textwid_->setFixedHeight (12);
    } else {
        textwid_->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
        textwid_->setFixedWidth (12);
    }
    layout ()->addWidget (textwid_);
    textwid_->setVisible (true);

    _colorChooser = new QColorDialog ();
}

// -----------------------------------------------------------
LegendEditor::~LegendEditor () {
    for (int i = 0; i < _sliders.size (); i++) delete (_sliders[i]);
    if (_colorChooser) { delete (_colorChooser); }
}

// -----------------------------------------------------------
int LegendEditor::getSliderCount () {
    return _sliders.size ();
}

// -----------------------------------------------------------
void LegendEditor::setColorChoose (QColorDialog* coldlg) {
    _colorChooser = coldlg;
}

// -----------------------------------------------------------
void LegendEditor::setSlideUpdate (bool val) {
    slideUpdate_ = val;
}

bool LegendEditor::SliderSort (const QColorRampEditorSlider* a1, const QColorRampEditorSlider* a2) {
    return a1->val < a2->val;
}

// -----------------------------------------------------------
Legend* LegendEditor::legend () {
    return _legend;
}

// -----------------------------------------------------------
void LegendEditor::showEvent (QShowEvent* e) {

    for (int i = 0; i < _sliders.size (); i++) delete (_sliders[i]);
    _sliders.clear ();

    // find min/max
    //min=legend -> entries().first().first;
    //max=legend -> entries().last().first;
    double range = max() - min();
    // create sliders
    for (int i = 0; i < _legend -> size(); i++) {
        QColorRampEditorSlider* sl = new QColorRampEditorSlider (_orientation, _legend -> at (i).second, slidewid_);
        sl->val = _legend -> at (i).first;
        _sliders.push_back (sl);
        updatePos (sl);
        sl->show ();
    }

    //emit legendChanged();
    slidewid_->update ();
    update ();
}

void LegendEditor::setMappingTextVisualize (bool vis) {
    visText_ = vis;
    textwid_->setVisible (visText_);
    update ();
}

void LegendEditor::setMappingTextColor (QColor col) {
    textColor_ = col;
    update ();
}

void LegendEditor::setMappingTextAccuracy (int acc) {
    textAcc_ = acc;
    update ();
}

qreal LegendEditor::updateValue (QColorRampEditorSlider* sl) {
    QRect crec = slidewid_->contentsRect ();
    if (_orientation == Qt::Horizontal) {
        crec.adjust (bspace_, 0, -bspace_, 0);
        sl -> val = min() + ((_zoom * (sl->pos ().x ()) - bspace_) / crec.width ()) * (max() - min());
    } else {
        crec.adjust (0, bspace_, 0, -bspace_);
        sl->val = min () + ((_zoom * (sl->pos ().y ()) - bspace_) / crec.height ()) * (max () - min ());
    }
    return sl->val;
}

int LegendEditor::updatePos (QColorRampEditorSlider* sl) {
    QRect crec = slidewid_->contentsRect ();
    qreal pos;
    if (_orientation == Qt::Horizontal) {
        crec.adjust (bspace_, 0, -bspace_, 0);
        pos = (sl->val - min()) / (max() - min()) * crec.width () / _zoom;
        pos += bspace_;
        sl->move (pos, 0);
    } else {
        crec.adjust (0, bspace_, 0, -bspace_);
        pos = (sl->val - min()) / (max() - min()) * crec.height () / _zoom;
        pos += bspace_;
        sl->move (0, pos);
    }
    return pos;
}




// -----------------------------------------------------------
void LegendEditor::setSliderColor (int index, QColor col) {
    if (index < 0 || index >= _sliders.size ()) { return; }
    _sliders[index]->setColor (col);
    emit legendChanged (legend() -> entries());
}

// -----------------------------------------------------------
void LegendEditor::resizeEvent (QResizeEvent* e) {
    for (int i = 0; i < _sliders.size (); i++) {
        QColorRampEditorSlider* sl = _sliders[i];
        updatePos (sl);
    }
}

// -----------------------------------------------------------
void LegendEditor::mousePressEvent (QMouseEvent* e) {
    if (e->button () == Qt::LeftButton) {
        QRect crec = contentsRect ();
        if (_orientation == Qt::Horizontal) {
            crec.adjust (bspace_, 0, -bspace_, 0);
            if (crec.contains (e->pos (), true)) {
                QColorRampEditorSlider* sl = new QColorRampEditorSlider (_orientation, Qt::white, slidewid_);
                _sliders.push_back (sl);
                sl -> move (e->pos ().x (), 0);
                //updateValue (sl);
                sl -> show ();
                qSort (_sliders.begin (), _sliders.end (), LegendEditor::SliderSort);
                updateRamp ();
            }
        } else {
            crec.adjust (0, bspace_, 0, -bspace_);
            if (crec.contains (e->pos (), true)) {
                QColorRampEditorSlider* sl = new QColorRampEditorSlider (_orientation, Qt::white, slidewid_);
                _sliders.push_back (sl);
                sl->move (0, e->pos ().y ());
                //updateValue (sl);
                sl->show ();
                qSort (_sliders.begin (), _sliders.end (), LegendEditor::SliderSort);
                updateRamp ();
            }
        }
    }
}

void LegendEditor::updateRamp () {
    _legend -> clear();
    for (QColorRampEditorSlider* slider : _sliders) {
        updatePos (slider);
        _legend -> addEntry (slider -> val, slider -> getColor());
    }

    if (textwid_-> isVisible ()) { textwid_->update (); }
    emit legendChanged (legend() -> entries());
    update ();
}

double LegendEditor::zoom() {
    return _zoom;
}

double LegendEditor::pan() {
    return _pan;
}

double LegendEditor::min () {
    return _pan - _zoom;
}

double LegendEditor::max () {
    return _pan + _zoom;
}


void LegendEditor::zoom (const int& steps) {
    _zoom *= std::pow (0.9, steps);
    updateRamp();
}

void LegendEditor::pan (const int& steps) {
    _pan -= 0.01 * _zoom * steps;
    updateRamp();
}

void LegendEditor::setView (double from, double to) {
    _pan = (from + to) / 2;
    _zoom = (to - from) / 2;
    updateRamp();
}

void LegendEditor::wheelEvent (QWheelEvent* e) {
    int clicks = e->delta () / 120;
    if (e -> modifiers() && Qt::ControlModifier) {
        zoom (clicks);
    } else {
        pan (clicks);
    }
}

// -----------------------------------------------------------
// QRampWidget -----------------------------------------------
// -----------------------------------------------------------

QRampWidget::QRampWidget (QWidget* parent) : QWidget (parent) {
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    setContentsMargins (0, 0, 0, 0);
    setMinimumHeight (5);
    setMinimumWidth (5);
}

void QRampWidget::paintEvent (QPaintEvent* e) {
    QPainter painter (this);
    painter.setPen (Qt::black);
    QColor color;

    double step = 2.0 / width() * _editor -> zoom();
    int i = 0;
    for (double index = _editor -> pan() - _editor -> zoom(); index < _editor -> pan() + _editor -> zoom(); index += step) {
        color = _editor -> _legend -> lookup (index);
        QPen pen = QPen (color);
        painter.setPen (pen);
        if (_editor->_orientation == Qt::Horizontal) {
            painter.drawLine (i, 0, i, height ());
        } else {
            painter.drawLine (0, i, width(), i);
        }
        i++;
    }

    QWidget::paintEvent (e);
}

// -----------------------------------------------------------
// QSlidersWidget --------------------------------------------
// -----------------------------------------------------------
QSlidersWidget::QSlidersWidget (QWidget* parent) : QWidget (parent), activeSlider_ (-1) {
    setContentsMargins (0, 0, 0, 0);
}

// -----------------------------------------------------------
void QSlidersWidget::mousePressEvent (QMouseEvent* e) {
    if (e->button () == Qt::LeftButton) {
        if (_editor->_sliders.size () < 2) { return; }
        for (int i = 1; i < _editor->_sliders.size () - 1; i++) {
            QRect srec = _editor->_sliders[i]->geometry ();
            if (srec.contains (e->pos (), true)) {
                activeSlider_ = i;
                break;
            }
        }
    }
}

// -----------------------------------------------------------
void QSlidersWidget::mouseMoveEvent (QMouseEvent* e) {
    if (activeSlider_ >= 0) {
        QRect crec = contentsRect ();

        qreal pos;
        if (_editor->_orientation == Qt::Horizontal) {
            crec.adjust (_editor->bspace_, 0, -_editor->bspace_, 0);
            pos = 1.0 * (e->pos ().x () - _editor->bspace_) / (crec.width ());
        } else {
            crec.adjust (0, _editor->bspace_, 0, -_editor->bspace_);
            pos = 1.0 * (e->pos ().y () - _editor->bspace_) / (crec.height ());
        }

        if (pos < 0.0 || pos > 1.0) {
            delete (_editor->_sliders[activeSlider_]);
            _editor->_sliders.removeAt (activeSlider_);
            activeSlider_ = -1;
            _editor->updateRamp ();
        } else {
            if (_editor->_orientation == Qt::Horizontal) {
                _editor->_sliders[activeSlider_]->move (e->pos ().x (), 0);
            } else {
                _editor->_sliders[activeSlider_]->move (0, e->pos ().y ());
            }

            _editor->updateValue (_editor->_sliders[activeSlider_]);
            qSort (_editor->_sliders.begin (), _editor->_sliders.end (), LegendEditor::SliderSort);
            if (_editor->slideUpdate_) { _editor->updateRamp (); }
        }
        update ();
    }
}

// -----------------------------------------------------------
void QSlidersWidget::mouseReleaseEvent (QMouseEvent* e) {
    if (e->button () == Qt::LeftButton) {
        activeSlider_ = -1;
        _editor->updateRamp ();
    }
}

// -----------------------------------------------------------
void QSlidersWidget::mouseDoubleClickEvent (QMouseEvent* e) {
    if (e->button () == Qt::LeftButton) {
        int index = -1;
        for (int i = 0; i < _editor->_sliders.size (); i++) {
            QRect srec = _editor->_sliders[i]->geometry ();
            if (srec.contains (e->pos (), true)) {
                index = i;
                break;
            }
        }
        if (index >= 0) {
            if (_editor->_colorChooser) {
                _editor->_colorChooser->exec ();
                //_editor->_sliders[index]->setColor (_editor->_colorChooser->selectedColor ());
                _editor -> setSliderColor (index, _editor -> _colorChooser -> selectedColor());
                _editor->updateRamp ();
            }
        }
    }
}


// -----------------------------------------------------------
// QColorRampEditorSlider ------------------------------------
// -----------------------------------------------------------
QColorRampEditorSlider::QColorRampEditorSlider (int orientation, QColor col, QWidget* parent) : QWidget (parent),
                                                                                                ori_ (orientation), color_ (col) {
    if (ori_ == Qt::Horizontal) {
        setFixedSize (9, 16);
    } else {
        setFixedSize (16, 9);
    }
}

// -----------------------------------------------------------
void QColorRampEditorSlider::setColor (QColor col) {
    color_ = col;
}

// -----------------------------------------------------------
QColor QColorRampEditorSlider::getColor () {
    return color_;
}

// -----------------------------------------------------------
void QColorRampEditorSlider::paintEvent (QPaintEvent* e) {
    QPainter painter (this);
    painter.setPen (Qt::black);
    painter.setBrush (color_);
    if (ori_ == Qt::Horizontal) {
        QRect rec (0, 7, 8, 8);
        painter.drawRect (rec);
        QPolygon pp;
        pp << QPoint (0, 7) << QPoint (4, 0) << QPoint (8, 7);
        painter.drawPolygon (pp, Qt::OddEvenFill);
    } else {
        QRect rec (7, 0, 8, 8);
        painter.drawRect (rec);
        QPolygon pp;
        pp << QPoint (7, 0) << QPoint (0, 4) << QPoint (7, 8);
        painter.drawPolygon (pp, Qt::OddEvenFill);
    }

}


// -----------------------------------------------------------
// QSliderTextWidget -----------------------------------------
// -----------------------------------------------------------
QSliderTextWidget::QSliderTextWidget (QWidget* parent) : QWidget (parent) {
    setContentsMargins (0, 0, 0, 0);
}

void QSliderTextWidget::paintEvent (QPaintEvent* e) {

    QPainter painter (this);

    QFont f = painter.font ();
    f.setPixelSize (8);
    painter.setFont (f);

    painter.setPen (Qt::black);
    painter.setBrush (_editor -> textColor_);

    QFontMetrics fm (f);

    // adjust size for vertical
    if (_editor->_orientation == Qt::Vertical) {
            QString txt1 = QString::number (_editor->_sliders.first ()->val, 'f', _editor->textAcc_);
            QString txt2 = QString::number (_editor->_sliders.last ()->val, 'f', _editor->textAcc_);
            int w = fm.width (txt1) + 4;
            if (w > this->width ()) { setFixedWidth (w); }
            w = fm.width (txt2) + 4;
            if (w > this->width ()) { setFixedWidth (w); }

        // draw the text for vertical orientation
        for (int i = 0; i < _editor->_sliders.size (); i++) {
            double pos = _editor->_sliders [i] -> pos().y();
            qreal val = _editor->_sliders[i]->val;
            QString txt = QString::number (val, 'f', _editor->textAcc_);
            painter.drawText (2, (int) pos + _editor->_sliders[i]->height (), txt);
        }
    } else {
        for (int i = 0; i < _editor->_sliders.size (); i++) {
            double pos = _editor->_sliders [i] -> pos().x();
            qreal val = _editor->_sliders[i]->val;
            QString txt = QString::number (val, 'f', _editor->textAcc_);
            if ((pos + fm.width (txt)) > width ()) { pos += -fm.width (txt) + _editor->_sliders[i]->width (); }
            painter.drawText ((int) pos, 2 + fm.height (), txt);
        }
    }

    QWidget::paintEvent (e);
}

