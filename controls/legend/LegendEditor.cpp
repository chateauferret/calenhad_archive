/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** QColorRampEditor is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/
#include "../../legend/Legend.h"
#include "LegendEditor.h"
#include "LegendPreview.h"
#include "LegendEditorSlider.h"
#include "LegendEditorSliderPanel.h"
#include "LegendEditorScale.h"
#include <cmath>
#include "CalenhadServices.h"
#include "LegendEntryDialog.h"
#include "exprtk/Calculator.h"

using namespace calenhad::controls::legend;
using namespace calenhad::legend;
using namespace calenhad::expressions;

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

    rampwid_ = new LegendPreview ();
    rampwid_->_editor = this;
    rampwid_->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    rampwid_->setContentsMargins (0, 0, 0, 0);

    layout() -> addWidget (rampwid_);

    slidewid_ = new LegendEditorSliderPanel ();
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

    _scale = new LegendEditorScale ();
    _scale -> setEditor (this);

    if (_orientation == Qt::Horizontal) {
        _scale->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
        _scale->setFixedHeight (12);
    } else {
        _scale->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
        _scale->setFixedWidth (12);
    }
    layout ()->addWidget (_scale);
    _scale->setVisible (true);

    _dialog = new LegendEntryDialog (this);
    //connect (CalenhadServices::calculator(), &Calculator::variableChanged, this, &LegendEditor::updateRamp);
}

// -----------------------------------------------------------
LegendEditor::~LegendEditor () {
    for (int i = 0; i < _sliders.size (); i++) delete (_sliders[i]);
    if (_dialog) { delete _dialog; }
}

// -----------------------------------------------------------
int LegendEditor::getSliderCount () {
    return _sliders.size ();
}

// -----------------------------------------------------------
void LegendEditor::setSlideUpdate (bool val) {
    slideUpdate_ = val;
}

bool LegendEditor::SliderSort (const LegendEditorSlider* a1, const LegendEditorSlider* a2) {
    return a1-> value() < a2-> value();
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
    double range = maximum() - minimum();
    // create sliders
    for (int i = 0; i < _legend -> size(); i++) {
        LegendEditorSlider* sl = new LegendEditorSlider (_orientation, _legend -> at (i).color(), slidewid_);
        sl -> setKey (_legend -> at (i).key());
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
    _scale->setVisible (visText_);
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

qreal LegendEditor::updateKey (LegendEditorSlider* sl) {
    QRect crec = slidewid_->contentsRect ();
    if (_orientation == Qt::Horizontal) {
        crec.adjust (bspace_, 0, -bspace_, 0);
        sl -> setKey (QString::number (valueAt (sl -> pos ().x () - bspace_)));
    } else {
        crec.adjust (0, bspace_, 0, -bspace_);
        sl-> setKey (QString::number (valueAt (sl->pos ().y () - bspace_)));
    }
    sl -> setToolTip (sl -> key() + " = " + QString::number (sl -> value()));
    return sl -> value();
}

int LegendEditor::updatePos (LegendEditorSlider* sl) {
    QRect crec = slidewid_-> contentsRect ();
    double pos = posForValue (sl -> value());
    pos += bspace_;
    if (_orientation == Qt::Horizontal) {
        sl -> move ((int) pos, 0);
    } else {
        sl -> move (0, (int) pos);
    }

    if (_scale -> isVisible ()) { _scale -> update(); }
    emit legendChanged (legend() -> entries());
    update ();
    return (int) pos;
}

void LegendEditor::setSlider (const int& index, const QString& key, const QColor& col) {
    if (index < 0 || index >= _sliders.size ()) { return; }
    _sliders [index] -> setColor (col);
    _sliders [index] -> setKey (key);
    _legend -> setEntry (index, key, col);
    emit legendChanged (legend() -> entries());
}

void LegendEditor::deleteSlider (const int& index) {
    delete (_sliders [index]);
    _sliders.removeAt (index);
    activeSlider_ = -1;
    updateRamp();
    emit legendChanged (legend() -> entries());
}

void LegendEditor::resizeEvent (QResizeEvent* e) {
    for (int i = 0; i < _sliders.size (); i++) {
        LegendEditorSlider* sl = _sliders[i];
        updatePos (sl);
    }
}

void LegendEditor::mousePressEvent (QMouseEvent* e) {
    if (e->button () == Qt::LeftButton) {
        QRect crec = contentsRect();
        if (_orientation == Qt::Horizontal) {
            crec.adjust (bspace_, 0, -bspace_, 0);
            if (crec.contains (e -> pos(), true)) {
                LegendEditorSlider* sl = new LegendEditorSlider (_orientation, Qt::white, slidewid_);
                _sliders.push_back (sl);
                sl -> move (e->pos().x(), 0);
                updateKey (sl);
                sl -> show();
                qSort (_sliders.begin(), _sliders.end (), LegendEditor::SliderSort);
                updateRamp();
            }
        } else {
            crec.adjust (0, bspace_, 0, -bspace_);
            if (crec.contains (e -> pos(), true)) {
                LegendEditorSlider* sl = new LegendEditorSlider (_orientation, Qt::white, slidewid_);
                _sliders.push_back (sl);
                sl->move (0, e -> pos().y());
                updateKey (sl);
                sl->show ();
                qSort (_sliders.begin(), _sliders.end(), LegendEditor::SliderSort);
                updateRamp();
            }
        }
    }
}

void LegendEditor::mouseMoveEvent (QMouseEvent* e) {
    if (_orientation == Qt::Horizontal) {
       setToolTip (QString::number (valueAt (e -> pos().x())));
    } else {
       setToolTip (QString::number (valueAt (e -> pos().y())));
    }
}

double LegendEditor::valueAt (const double& pos) {
    QRect crec = slidewid_->contentsRect();
    crec.adjust (bspace_, 0, -bspace_, 0);
    return minimum() + ((pos / crec.width()) * (maximum() - minimum()));
}

double LegendEditor::posForValue (const double& value) {
    QRect crec = slidewid_-> contentsRect();
    crec.adjust (bspace_, 0, -bspace_, 0);
    return (value - minimum()) / (maximum() - minimum()) * crec.width();
}

void LegendEditor::updateRamp () {
    _legend -> clear();
    for (LegendEditorSlider* slider : _sliders) {
        updatePos (slider);
        _legend -> addEntry (LegendEntry (slider -> key(), slider-> color()));
    }

    if (_scale-> isVisible ()) { _scale -> update(); }
    emit legendChanged (legend() -> entries());
    update ();
}

double LegendEditor::zoom() {
    return _zoom;
}

double LegendEditor::pan() {
    return _pan;
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

double LegendEditor::minimum () {
    return _pan - _zoom;
}

double LegendEditor::maximum () {
    return _pan + _zoom;
}