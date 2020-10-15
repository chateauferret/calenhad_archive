//
// Created by ferret on 12/10/2020.
//

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCore/QFile>
#include <QApplication>
#include <QtWidgets/QFileDialog>
#include "CubeRasterFile.h"
#include "CubeRasterModule.h"

using namespace calenhad::module;


CubeRasterFile::CubeRasterFile (CubeRasterModule* module) : _index (0),
    _rotation (0), _image (nullptr), _ui (nullptr), _filenameLabel (nullptr), _filename (QString::null),
    _module (module) {
    _rotations [0].rotate (0);
    _rotations [2].rotate (180);
    _rotations [1].rotate (90);
    _rotations [3].rotate (-90);
    _arrows [0] = QIcon (":/appicons/controls/arrow_up.png");
    _arrows [1] = QIcon (":/appicons/controls/arrow_right.png");
    _arrows [2] = QIcon (":/appicons/controls/arrow_down.png");
    _arrows [3] = QIcon (":/appicons/controls/arrow_left.png");
    _ui = new QWidget();
    QHBoxLayout* rowLayout = new QHBoxLayout();
    _ui -> setLayout (rowLayout);
    _filenameLabel = new QLabel();
    _filenameLabel -> setMinimumWidth (300);
    _filenameLabel -> setText ("No file selected");
    _filenameLabel -> setToolTip (_filename);
    rowLayout -> addWidget (_filenameLabel);
    QPushButton* fileButton = new QPushButton();
    fileButton -> setFixedWidth (64);
    fileButton -> setText ("...");
    connect (fileButton, &QPushButton::pressed, this, [=] () {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        QString dir;
        if (_filename != QString::null) {
            QDir f (_filename);
            dir = f.dirName();
        }
        _filename = QFileDialog::getOpenFileName (module, "Select raster", dir, "Image Files (*.png *.jpg *.bmp)");
        _filenameLabel -> setText (_filename.isNull() ? "No file selected" : _filename);
        _filenameLabel -> setToolTip (_filename);
        _image = new QImage (_filename);
        for (int i = 0; i < _rotation; i++) { rotateRight(); }
        //QPixmap pixmap = QPixmap::fromImage (*_image).scaled (_filenameLabel -> size());
        //_filenameLabel -> setPixmap (pixmap);
        _module -> invalidate();
        QApplication::restoreOverrideCursor();
    });
    rowLayout -> addWidget (fileButton);
    _rotationButton = new QPushButton();
    _rotationButton -> setFixedWidth (64);
    connect (_rotationButton, &QPushButton::pressed, this, [=] () {
        rotateRight();
    });
    rowLayout -> addWidget (_rotationButton);
}

CubeRasterFile::~CubeRasterFile() {
    delete _image;
}

const QString& CubeRasterFile::getFilename() const {
    return _filename;
}

void CubeRasterFile::setFilename (const QString& filename) {
    _filename = filename;
    _filenameLabel -> setText (filename.isNull() ? "No file selected" : _filename);
    _filenameLabel -> setToolTip (_filename);
    delete _image;
    _image = filename.isNull() ? nullptr : new QImage (filename);
}

int CubeRasterFile::getIndex() const {
    return _index;
}

void CubeRasterFile::setIndex (int index) {
    _index = index;
}

int CubeRasterFile::getRotation() const {
    return _rotation;
}

void CubeRasterFile::rotateRight() {
    _rotation++;
    if (_rotation > 3) { _rotation -= 4; }
    if (_image) {
        QImage newImage (_image -> transformed (_rotations [1]));
        delete _image;
        _image = new QImage (newImage);
    }
    updateUi();
}

void CubeRasterFile::rotateLeft() {
    _rotation--;
    if (_rotation < 0) { _rotation += 4; }
    if (_image) {
        QImage newImage (_image -> transformed (_rotations [3]));
        delete _image;
        _image = new QImage (newImage);
    }
    updateUi();
}

void CubeRasterFile::updateUi() {
    _rotationButton -> setIcon (_arrows [_rotation]);
    _module -> invalidate();
}

QImage* calenhad::module::CubeRasterFile::getImage() const {
    return _image;
}

QWidget* CubeRasterFile::ui () {
    return _ui;
}

void CubeRasterFile::setRotation (int i) {
    _rotation = i % 4;
    if (_image) {
        QImage newImage (_image -> transformed (_rotations [_rotation]));
        delete _image;
        _image = new QImage (newImage);
    }
    updateUi();

}

