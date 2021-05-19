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


CubeRasterFile::CubeRasterFile() : _index (0),
    _rotation (0), _image (nullptr), _ui (nullptr), _filenameLabel (nullptr), _rotationButton (new QPushButton()), _filename (QString()),
    _flipVerticalButton (new QPushButton()), _flipHorizontalButton (new QPushButton()),
    _flipVertical (false), _flipHorizontal (false), _enabled (true) {
    _rotations [0].rotate (0);
    _rotations [2].rotate (180);
    _rotations [1].rotate (90);
    _rotations [3].rotate (-90);
    _arrows [0] = QIcon (":/appicons/controls/arrow_up.png");
    _arrows [1] = QIcon (":/appicons/controls/arrow_right.png");
    _arrows [2] = QIcon (":/appicons/controls/arrow_down.png");
    _arrows [3] = QIcon (":/appicons/controls/arrow_left.png");
    _flipHorizontalButton -> setIcon (QIcon (":/appicons/controls/flip_horizontal.png"));
    _flipVerticalButton -> setIcon (QIcon (":/appicons/controls/flip_vertical.png"));
    _flipHorizontalButton -> setCheckable (true);
    _flipHorizontalButton -> setCheckable (false);
    _flipVerticalButton -> setCheckable (true);
    _flipVerticalButton -> setCheckable (false);

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
        if (_filename != QString()) {
            QDir f (_filename);
            dir = f.dirName();
        }
        _filename = QFileDialog::getOpenFileName (_ui, "Select raster", dir, "Image Files (*.png *.jpg *.bmp)");
        _filenameLabel -> setText (_filename.isNull() ? "No file selected" : _filename);
        _filenameLabel -> setToolTip (_filename);
        _image = new QImage (_filename);

        //QPixmap pixmap = QPixmap::fromImage (*_image).scaled (_filenameLabel -> size());
        //_filenameLabel -> setPixmap (pixmap);
        QApplication::restoreOverrideCursor();
    });
    rowLayout -> addWidget (fileButton);

    _rotationButton -> setFixedWidth (64);
    connect (_rotationButton, &QPushButton::pressed, this, [=] () {setRotation (_rotation + 1); });
    connect (_flipHorizontalButton, &QPushButton::toggled, this, &CubeRasterFile::flipHorizontal);
    connect (_flipVerticalButton, &QPushButton::toggled, this, &CubeRasterFile::flipVertical);
    rowLayout -> addWidget (_rotationButton);
    rowLayout -> addWidget (_flipHorizontalButton);
    rowLayout -> addWidget (_flipVerticalButton);
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

void CubeRasterFile::flipHorizontal (const bool& flip) {
    _flipHorizontal = flip;
    updateImage();
}

void CubeRasterFile::flipVertical (const bool& flip) {
    _flipVertical = flip;
}

QImage* calenhad::module::CubeRasterFile::getImage() const {
    return _image;
}

void CubeRasterFile::setRotation (int i) {
    _rotation = i % 4;
    updateImage();
}

QString CubeRasterFile::filename() {
    return _filename;
}

bool CubeRasterFile::isFlippedVertical() const {
    return _flipVertical;
}

bool CubeRasterFile::isFlippedHorizontal() const {
    return _flipHorizontal;
}

bool CubeRasterFile::isEnabled () const {
    return _enabled;
}

void CubeRasterFile::updateImage () {
    if (_image) {
        QImage newImage = _image -> mirrored (_flipHorizontal, _flipVertical).transformed (_rotations [_rotation]);
        delete _image;
        _image = new QImage (newImage);
    }
}

void CubeRasterFile::setEnabled (const bool& enabled) {
    _enabled = enabled;
    updateImage();
}

