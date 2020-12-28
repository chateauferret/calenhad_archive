/****************************************************************************
** Includes code which is
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
#include "src/CalenhadServices.h"
#include "LegendEntryDialog.h"
#include "exprtk/Calculator.h"

using namespace calenhad::controls::legend;
using namespace calenhad::legend;
using namespace calenhad::expressions;

LegendEditor::LegendEditor (Legend* legend, QWidget* parent, int orientation) : QWidget (parent), _legend (legend),
     _orientation (orientation), _bspace (5),
    activeSlider_ (-1), _slideUpdate (false),
    _zoom (1.0), _pan (0.0) {
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
    layout() -> setMargin (0);
    layout() -> setSpacing (0);
    layout() -> setContentsMargins (0, 0, 0, 0);

    _rampWidget = new LegendPreview ();
    _rampWidget -> _editor = this;
    _rampWidget -> setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    _rampWidget -> setContentsMargins (0, 0, 0, 0);

    layout() -> addWidget (_rampWidget);

    _sliderWidget = new LegendEditorSliderPanel (this);

    if (_orientation == Qt::Horizontal) {
        _sliderWidget->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
        _sliderWidget->setFixedHeight (16);
    } else {
        _sliderWidget->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
        _sliderWidget->setFixedWidth (16);
    }
    _sliderWidget->setContentsMargins (0, 0, 0, 0);
    layout() -> addWidget (_sliderWidget);

    _scale = new LegendEditorScale (_sliderWidget);

    if (_orientation == Qt::Horizontal) {
        _scale->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
        _scale->setFixedHeight (12);
    } else {
        _scale->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
        _scale->setFixedWidth (12);
    }
    layout() -> addWidget (_scale);
    _scale -> setVisible (true);

}

LegendEditor::~LegendEditor () {

}

void LegendEditor::setSlideUpdate (bool val) {
    _slideUpdate = val;
}

Legend* LegendEditor::legend () {
    return _legend;
}

void LegendEditor::showEvent (QShowEvent* e) {

    _sliderWidget -> createSliders (_legend);
    update();
}

void LegendEditor::setMappingTextVisualize (bool vis) {
    visText_ = vis;
    _scale -> setVisible (visText_);
    update ();
}

void LegendEditor::setMappingTextColor (QColor col) {
    textColor_ = col;
    update ();
}

void LegendEditor::setMappingTextAccuracy (int acc) {
    textAcc_ = acc;
    update();
}


void LegendEditor::resizeEvent (QResizeEvent* e) {
    _sliderWidget -> updateSliders();
}

void LegendEditor::mousePressEvent (QMouseEvent* e) {
    if (e->button () == Qt::LeftButton) {
        QRect crec = contentsRect();
        if (crec.contains (e -> pos(), true)) {
            _sliderWidget -> addSlider (_orientation, _orientation == Qt::Horizontal ? e -> pos().x() : e -> pos().y());
        }
    }
}

void LegendEditor::mouseMoveEvent (QMouseEvent* e) {
    if (_orientation == Qt::Horizontal) {
       setToolTip (QString::number (_sliderWidget -> valueAt (e -> pos().x())));
    } else {
       setToolTip (QString::number (_sliderWidget -> valueAt (e -> pos().y())));
    }
}

void LegendEditor::updateRamp () {
    _legend -> clear();
    _sliderWidget -> updateSliders();
    _sliderWidget -> createSliders (_legend);
    if (_scale -> isVisible ()) { _scale -> update(); }
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

double LegendEditor::minimum () const {
    return _pan - _zoom;
}

double LegendEditor::maximum () const {
    return _pan + _zoom;
}

int LegendEditor::orientation () const {
    return _orientation;
}

double LegendEditor::valueAt (const int& i) {
    return _sliderWidget -> valueAt (i);
}
