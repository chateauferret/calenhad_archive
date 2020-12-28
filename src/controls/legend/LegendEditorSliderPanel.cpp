//
// Created by martin on 27/09/17.
//

#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include "LegendEditorSliderPanel.h"
#include "LegendEditorSlider.h"
#include "LegendEditorScale.h"
#include "LegendEditor.h"
#include "../../legend/Legend.h"

using namespace calenhad::controls::legend;
using namespace calenhad::legend;

LegendEditorSliderPanel::LegendEditorSliderPanel (LegendEditor* parent) : QWidget (parent), _activeSlider (-1), _bspace (5), _editor (parent) {
    setContentsMargins (0, 0, 0, 0);
    setMouseTracking (true);
}

void LegendEditorSliderPanel::mousePressEvent (QMouseEvent* e) {

    if (e -> button() == Qt::LeftButton) {
        for (int i = 0; i < _sliders.size(); i++) {
            QRect srec = _sliders [i] -> geometry ();
            if (srec.contains (e -> pos())) {
                // only allow an entry to be dragged _about if its key is a literal value, so as not to destroy expressions
                if (! (_editor -> _legend -> entries().at(i).isComputed())) {
                    _activeSlider = i;
                    _sliders [i] -> setCursor (Qt::ClosedHandCursor);
                }
                break;
            }
        }
    }
}

void LegendEditorSliderPanel::mouseMoveEvent (QMouseEvent* e) {

    if (_activeSlider >= 0) {
        QRect crec = contentsRect();

        qreal pos;
        if (_editor->_orientation == Qt::Horizontal) {
            crec.adjust (_bspace, 0, -_bspace, 0);
            pos = 1.0 * (e -> pos().x() - _bspace) / (crec.width());
        } else {
            crec.adjust (0, _bspace, 0, -_bspace);
            pos = 1.0 * (e -> pos().y() - _bspace) / (crec.height());
        }

        if (crec.contains (e -> pos()))  {
            _sliders[_activeSlider] -> assignCursor();
            if (_editor -> _orientation == Qt::Horizontal) {
                _sliders [_activeSlider] -> move (e -> pos().x(), 0);
            } else {
                _sliders [_activeSlider] -> move (0, e -> pos().y());
            }

        } else {
            _sliders [_activeSlider] -> setCursor (Qt::ForbiddenCursor);
        }
    }
}

void LegendEditorSliderPanel::mouseReleaseEvent (QMouseEvent* e) {
    if (e -> button() == Qt::LeftButton) {
        if (_activeSlider >= 0) {
            if (rect().contains (e -> pos())) {
                _sliders [_activeSlider] -> assignCursor();
                updateKey (_sliders[_activeSlider]);
            } else {
                // delete the slider if it was dragged off the panel
	            deleteSlider (_activeSlider);
            }
            std::sort (_sliders.begin(), _sliders.end(), SliderSort);
            _activeSlider = -1;
            _editor -> updateRamp();
        }
    }

    if (e -> button() == Qt::RightButton) {
        int index = -1;
        for (int i = 0; i < _sliders.size (); i++) {
            QRect srec = _sliders [i] -> geometry();
            if (srec.contains (e -> pos(), true)) {
                index = i;
                break;
            }
        }
        if (index >= 0) {
            LegendEntryDialog* dialog = new LegendEntryDialog (this);
            dialog -> setColor (_editor -> _legend -> entries().at (index).color());
            dialog -> setIndex (_editor -> _legend -> entries().at (index).key());
            dialog -> preventDelete (_editor -> _legend -> entries().size() < 3);
            if (dialog -> exec () == QDialog::Accepted) {
                if (dialog -> isDelete()) {
                    deleteSlider (index);
                } else {
                    QColor color = dialog -> color();
                    QString key = dialog -> index();
                    LegendEntry entry = _editor -> _legend -> entries().at (index);
                    setSlider (index, key, color);
                }
            }
            delete dialog;

        }
    }
}


bool LegendEditorSliderPanel::SliderSort (const LegendEditorSlider* a1, const LegendEditorSlider* a2) {
    return a1 -> value() < a2 -> value();
}

double LegendEditorSliderPanel::valueAt (const double& pos) {
    QRect crec = contentsRect();
    crec.adjust (_bspace, 0, -_bspace, 0);
    return _editor -> minimum() + ((pos / crec.width()) * (_editor -> maximum() - _editor -> minimum()));
}

double LegendEditorSliderPanel::posForValue (const double& value) {
    QRect crec = contentsRect();
    crec.adjust (_bspace, 0, -_bspace, 0);
    return (value - _editor -> minimum()) / (_editor -> maximum() - _editor -> minimum()) * crec.width();
}


qreal LegendEditorSliderPanel::updateKey (LegendEditorSlider* sl) {
    QRect crec = contentsRect();
    if (_editor -> orientation() == Qt::Horizontal) {
        crec.adjust (_bspace, 0, -_bspace, 0);
        sl -> setKey (QString::number (valueAt (sl -> pos().x() - _bspace)));
    } else {
        crec.adjust (0, _bspace, 0, -_bspace);
        sl-> setKey (QString::number (valueAt (sl -> pos().y() - _bspace)));
    }
    sl -> setToolTip (sl -> key() + " = " + QString::number (sl -> value()));
    return sl -> value();
}

int LegendEditorSliderPanel::updatePos (LegendEditorSlider* sl) {
    QRect crec = contentsRect();
    double pos = posForValue (sl -> value());
    pos += _bspace;
    if (_editor -> orientation() == Qt::Horizontal) {
        sl -> move ((int) pos, 0);
    } else {
        sl -> move (0, (int) pos);
    }

    update ();
    return (int) pos;
}


void LegendEditorSliderPanel::setSlider (const int& index, const QString& key, const QColor& col) {
    if (index < 0 || index >= _sliders.size()) { return; }
    _sliders [index] -> setColor (col);
    _sliders [index] -> setKey (key);
    updatePos (_sliders [index]);
    std::sort (_sliders.begin(),  _sliders.end(), SliderSort);
    bool ok;
    key.toDouble (&ok);
    _sliders [index] -> setComputed (! ok);
    _editor -> updateRamp();
}

void LegendEditorSliderPanel::deleteSlider (const int& index) {
    delete (_sliders[index]);
    _sliders.removeAt (index);
    _activeSlider = -1;
    _editor -> updateRamp();
}


void LegendEditorSliderPanel::createSliders (Legend* legend) {

     for (int i = 0; i < _sliders.size (); i++) delete (_sliders[i]);
    _sliders.clear ();

    // create sliders
    for (int i = 0; i < legend -> size(); i++) {
        LegendEditorSlider* sl = new LegendEditorSlider (_editor -> orientation(), legend -> at (i).color(), this);
        QString key = legend -> at (i).key();
        sl -> setKey (key);
        bool ok;
        key.toDouble (&ok);
        sl -> setComputed (! ok);
        _sliders.push_back (sl);
        updatePos (sl);
        sl -> show ();
    }
}

void LegendEditorSliderPanel::updateSliders () {
    for (LegendEditorSlider* slider : _sliders) {
        updatePos (slider);
    }
}

QVector<LegendEntry> LegendEditorSliderPanel::entries() {
    QVector<LegendEntry> entries;
    for (LegendEditorSlider* slider : _sliders) {
        LegendEntry entry (slider -> key(), slider -> color());
        entries.push_back (entry);
    }
    return entries;
}

void LegendEditorSliderPanel::addSlider (const int& orientation, const int& pos) {
    LegendEditorSlider* sl = new LegendEditorSlider (orientation, Qt::white, this);
    _sliders.push_back (sl);
    sl -> move (orientation == Qt::Horizontal ? pos : 0, orientation == Qt::Horizontal ? 0 : pos);
    updateKey (sl);
    sl -> show();
    std::sort (_sliders.begin(), _sliders.end (), SliderSort);
    _editor -> updateRamp();
}

int LegendEditorSliderPanel::orientation () {
    return _editor -> orientation();
}

LegendEditorSliderPanel::~LegendEditorSliderPanel () {
    for (LegendEditorSlider* s : _sliders) {
        delete s;
    }
}
