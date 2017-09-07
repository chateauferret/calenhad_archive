//
// Created by martin on 10/08/17.
//

#include "CalenhadMap.h"
#include "CalenhadMapWidget.h"
using namespace calenhad::mapping;
CalenhadMap::CalenhadMap () : QMainWindow() {
    _widget = new CalenhadMapWidget (this);
    setCentralWidget (_widget);
}

CalenhadMap::~CalenhadMap () {

}

void CalenhadMap::setGraph (calenhad::graph::Graph* g) {
    _widget -> setGraph (g);
}

/*
void CalenhadMap::initializeGL () {
    QOpenGLWindow::initializeGL ();
}

void CalenhadMap::resizeGL (int width, int height) {
    QOpenGLWindow::resizeGL (width, height);
}

void CalenhadMap::paintGL () {
    QOpenGLWindow::paintGL ();
}
*/