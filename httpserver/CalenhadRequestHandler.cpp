//
// Created by martin on 06/01/17.
//

#include <QtGui/QImage>
#include <sstream>
#include "CalenhadRequestHandler.h"
#include "../nodeedit/qnemainwindow.h"

using namespace qtwebapp;


CalenhadRequestHandler::CalenhadRequestHandler (QObject* parent) : HttpRequestHandler (parent),
    _size (TileProducer::DEFAULT_SIZE),
    _buffer (new QBuffer()),
    _content (new QByteArray()) {
}

CalenhadRequestHandler::~CalenhadRequestHandler() {
    if (_content) { delete _content; }
    if (_buffer) { delete _buffer; }
}

void CalenhadRequestHandler::service (HttpRequest& request, HttpResponse& response) {
    if (! (_tile)) {
        _tile = std::make_shared<QImage> (_size, _size, QImage::Format::Format_RGB32);
    }
    QByteArray message;
    if (request.getMethod() == "POST") {
        // to do
    } else {
        if (request.getMethod() == "GET") {
            QByteArray message, statusMessage;
            int statusCode;
            parseTilePath (request.getPath(), message, statusCode, statusMessage, _content);
            response.setStatus (statusCode, statusMessage);
            if (statusCode == 200) {
                response.write (*_content);
            } else {
                response.write (message);
            }
        } else {
            response.setStatus (501, "Unsupported");
        }
    }
}

void CalenhadRequestHandler::setTileSize (const unsigned& s) {
    _size = s;
    _tile = std::make_shared<QImage> (_size, _size, QImage::Format::Format_RGB32);
}

void CalenhadRequestHandler::parseTilePath (const QByteArray& path, QByteArray& message, int& statusCode, QByteArray& statusMessage, QByteArray* content) {
    int x, y, z;
    QByteArray format, dataset;

    // format of tile path is:
    // {servername}:{port}/tile/{format}/{module_name}/{x}/{y}/{z}
    std::cout << "Request for " << path.toStdString () << "\n";
    QList<QByteArray> params = path.split ('/');

    if (params.size() != 7) {
        message = "Request in incorrect format.";
        statusCode = 400;
        statusMessage = "Bad request";
        return;
    }

    // check that request is really for tiles and not something else
    if (params.at (CalenhadRequestPart::Service).toLower() != "tile") {
        message = "Request sent to tile request handler but request is for " + params.at (CalenhadRequestPart::Service).toLower() + ".";
        statusCode = 404;
        statusMessage = "Not found";
        return;
    }

    // obtain the module whose output is to be served
    QString moduleName = params.at (CalenhadRequestPart::ModuleName);
    noise::module::Module* module = findModule (moduleName);
    if (! (module)) {
        std::ostringstream messageStream;
        messageStream << "Module not found.\n";
        message = QByteArray::fromStdString (messageStream.str());
        statusCode = 404;
        statusMessage = "Not found";
        return;
    }

    // identify requested file format
    QList<QByteArray> fileNameParts = params.at (CalenhadRequestPart::Format).split ('.');
    bool ok = true;
    if (fileNameParts.size() < 2) {
        format = fileNameParts.at (0).toLower();
    } else {
        format = fileNameParts.at (1).toLower();
        if (format != "png") {
            message = ("Format not supported.\n Available formats are: PNG");
            statusCode = 404;
            statusMessage = "Not found";
            return;
        }
    }
    if (ok) { z = params.at (CalenhadRequestPart::TileZ).toInt (&ok); }
    if (z < 0 || z > RenderJob::MAX_ZOOM) {
        std::cout << z << "\n";
        message = "Zoom out of range.\n Available zoom is 0 to 24";
        statusCode = 400;
        statusMessage = "Bad request";
        return;
    }
    if (ok) { x = params.at (CalenhadRequestPart::TileX).toInt (&ok); }
    if (ok) { y = params.at (CalenhadRequestPart::TileY).toInt (&ok); }


    // identify requested zoom level
    z = params.at (CalenhadRequestPart::TileZ).toInt (&ok);
    if (! ok) {
        message = "Zoom must be an integer.\n Available zoom is 0 to 24";
        statusCode = 400;
        statusMessage = "Bad request";
        return;
    }

    if (ok && _tile != 0) {
        TileProducer tileProducer (module, moduleName);
        std::shared_ptr<QImage> tile = tileProducer.fetchTile (x, y, z);
        if (tile) {
            _buffer -> setBuffer (content);
            _buffer -> open  (QIODevice::WriteOnly);
            _tile -> save (_buffer, format);
            _buffer -> close();
            statusCode = 200;
            statusMessage = "OK";
            return;
        } else {
            message = "Module returned no data";
            statusCode = 500;
            statusMessage = "Internal error";
            return;
        }
    } else {
        std::ostringstream messageStream;
        messageStream << "Tile coordinates out of bounds.\n At zoom " << z << ", tile coordinates are between 0 and " << TileProducer::powers (z);
        message = QByteArray::fromStdString (messageStream.str());
        statusCode = 400;
        statusMessage = "Bad request";
    }
}

noise::module::Module* CalenhadRequestHandler::findModule (const QString& name) {
    QModule* qm = _model -> findModule (name);
    if (qm) {
        return qm -> module ();
    } else {
        return nullptr;
    }
}

void CalenhadRequestHandler::setModel (CalenhadModel* model) {
    _model = model;
}
