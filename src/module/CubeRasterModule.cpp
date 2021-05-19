//
// Created by ferret on 11/10/2020.
//

#include "CubeRasterModule.h"

//
// Created by martin on 07/10/2019.
//

#include <QtWidgets/QFileDialog>
#include <QtGui/QPainter>
#include "RasterModule.h"
#include "geoutils.h"
#include <QApplication>
#include "../messages/QProgressNotification.h"
#include "CubeRasterFile.h"
#include "CubeRasterDialog.h"


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;
using namespace calenhad::mapping;
using namespace calenhad::grid;
using namespace geoutils;
using namespace calenhad::notification;

CubeRasterModule::CubeRasterModule (const QString& type) : RasterModule (type), _index (0), _rotation (0), _dialog (nullptr) {
    for (int i = 0; i < 6; i++) {
        CubeRasterFile* file = new CubeRasterFile();
        file -> setIndex (i);
        _files.append (file);
    }
}

CubeRasterModule::~CubeRasterModule() {
    for (CubeRasterFile* f : _files) {
        delete f;
    }
    delete _dialog;
}

QImage* CubeRasterModule::image (int i) {
    if (_files.size() < i) {
        return nullptr;
    } else {
        return _files.at (i) -> getImage();
    }
}

QList<QImage*> CubeRasterModule::images() {
    QList<QImage*> list;
    for (int i = 0; i < 6; i++) {
        QImage* image = _files [i] -> getImage();
        list.append (image);
    }
    return list;
}


void CubeRasterModule::inflate (const QDomElement& element) {
    RasterModule::inflate (element);
    QDomElement rasterElement = element.firstChildElement ("filenames");
    QDomNodeList nodes = rasterElement.childNodes();

    for (int i = 0; i < std::max (6, nodes.size()); i++) {
        QDomElement fileElement = nodes.at (i).toElement();
        bool ok = false;
        CubeRasterFile* file = _files.at (i);
        int index = fileElement.attribute ("index").toInt (&ok);
        if (ok) {
            file -> setIndex (index);
            file -> setFilename (fileElement.attribute ("filename"));
            int rotation = fileElement.attribute ("rotation").toInt (&ok);
            if (ok) {
                file -> setRotation (rotation);
            }
        }
    }
    invalidate();
}

void CubeRasterModule::serialize (QDomElement& element) {
    RasterModule::serialize (element);
    QDomElement rasterElement = _document.createElement ("filenames");
    if (_dialog) {
        for (int i = 0; i < 6; i++) {
            CubeRasterFile* file = _files.at (i);
            if (file && ! file -> getFilename ().isNull ()) {
                QDomElement fileElement = _document . createElement ("file");
                fileElement . setAttribute ("index", QString::number (i));
                fileElement . setAttribute ("filename", file -> getFilename ());
                fileElement . setAttribute ("rotation", file -> getRotation ());
                rasterElement . appendChild (fileElement);
            }
        }
    }
    _element.appendChild (rasterElement);
}

// for now, suppose we are complete only if we have at least one image
bool CubeRasterModule::isComplete() {
    if (! _dialog) {
        return false;
    }
    for (int i = 0; i < 6; i++) {
        QImage* f = _files.at (i) -> getImage();
        if (f && _buffer) {
            return RasterModule::isComplete();;
        }
    }
    return false;
}

void CubeRasterModule::invalidate () {
    if (! _buffer) {
        bool ok;
        double cubeSize = (double) parameter ("resolution").toInt (&ok);
        _buffer = new CubicSphere (ok ? (int) std::log2 (cubeSize) - 2 : 10);
    }
    _buffer -> fromRasters (images());
    RasterModule::invalidate ();
}

void CubeRasterModule::fileDialogRequested () {
    if (! _dialog) {
        _dialog = new CubeRasterDialog (this);
    }
    if (_dialog -> exec()) {
        invalidate();
    };
}

QList<CubeRasterFile*> CubeRasterModule::files () {
    return _files;
}
