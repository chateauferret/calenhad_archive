//
// Created by martin on 06/06/17.
//

#include "QNodeGroup.h"
#include "../pipeline/CalenhadModel.h"


QNodeGroup::QNodeGroup (QWidget* parent) : QNode (parent), _rect (QRect (0, 0, 100, 100)) {
    std::cout << "New QNodeGroup\n";
}

QNodeGroup::~QNodeGroup () {

}

void QNodeGroup::setRect (const QRect& rect) {
    _rect = rect;
}

QRect QNodeGroup::rect () {
    return _rect;
}

bool QNodeGroup::isWithin (const QPoint& point) {
    return _rect.contains (point);
}


void QNodeGroup::setUniqueName() {
    int i = 0;
    if (_name.isNull()) {
        _name = "New module group";
    }
    QString root = _name;
    QString name = root;
    do  {
        name = QString (root + QString::number (++i));
    } while (_model -> findGroup (name));
    _name = name;
}

void QNodeGroup::addInputPorts () {
    // do nothing for now - groups don't have ports
}

QString QNodeGroup::nodeType () {
    return "NodeType";
}

QNodeGroup* QNodeGroup::clone () {
    QNodeGroup* other = new QNodeGroup ((QWidget*) parent());
    other -> setRect (_rect);
    other -> setName (_name);
    other -> setUniqueName();
    return other;
}

void QNodeGroup::initialise() {
    QNode::initialise();
    _octaveSpin = countParameterControl ("Number of octaves");
    _octaveSpin -> setValue (getOctaveCount());
    _contentLayout -> addRow ("Octaves", _octaveSpin);
    setOctaveCount (getOctaveCount ());
}


int QNodeGroup::getOctaveCount () {
    return _octaves;
}

void QNodeGroup::setOctaveCount (const int& value) {
    _octaves = value;
    emit changedOctaves (_octaves);
    _octaveSpin -> setValue (value);

}