//
// Created by ferret on 14/10/2020.
//

#include <QtWidgets/QVBoxLayout>
#include "CubeRasterDialog.h"
#include "CubeRasterFile.h"

using namespace calenhad::module;


CubeRasterDialog::CubeRasterDialog (CubeRasterModule* parent) : QDialog () {
    setLayout (new QVBoxLayout());
    for (int i = 0; i < 6; i++) {
        CubeRasterFile* file = new CubeRasterFile (parent);
        file -> setIndex (i);
        _files.append (file);
        layout() -> addWidget (file -> ui());
    }

}

CubeRasterDialog::~CubeRasterDialog () {
    for (CubeRasterFile* f : _files) {
        delete f;
    }
}


QList<QImage*> CubeRasterDialog::images() {
    QList<QImage*> list;
    for (int i = 0; i < 6; i++) {
        QImage* image = _files [i] -> getImage();
        list.append (image);
    }
    return list;
}

QImage* CubeRasterDialog::image (int i) {
    if (_files.size() < i) {
        return nullptr;
    } else {
        return _files.at (i) -> getImage();
    }
}

void CubeRasterDialog::populate (const QList<CubeRasterFile*>& list) {

    for (int i = 0; i < 6; i++) {
        if (list.size() >= i) {
            _files.at (i) -> setFilename (list.at (i) -> getFilename());
            _files.at (i) -> setRotation (list.at (i) -> getRotation());
            _files.at (i) -> setIndex (i);
        } else {
            _files.at (i) -> setFilename (QString::null);
            _files.at (i) -> setIndex (i);
        }
    }
}

CubeRasterFile* CubeRasterDialog::file (const int& i) {
    return _files [i];
}
