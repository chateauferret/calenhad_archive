//
// Created by martin on 06/06/17.
//

#include "QNodeGroup.h"
#include "../pipeline/CalenhadModel.h"


QNodeGroup::QNodeGroup (QWidget* parent) : QNode (parent), _rect (QRect (0, 0, 100, 100)) {

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
    QString name = "New module group";

    while (_model -> findGroup (name)) {
        i++;
        name = QString ("New module group " + QString::number (i));
    }
    setName (name);
}

void QNodeGroup::addInputPorts () {
    // do nothing for now - groups don't have ports
}
