//
// Created by martin on 06/01/17.
//
// Using the CMake port of QtHttpServer by Stefan Frings
// https://github.com/msrd0/QtWebApp

#ifndef CALENHAD_CALENHADREQUESTHANDLER_H
#define CALENHAD_CALENHADREQUESTHANDLER_H
#include <httprequesthandler.h>
#include <httprequest.h>
#include <httpresponse.h>
#include <QtCore/QBuffer>
#include <libnoise/module/modulebase.h>
#include "../geoutils.h"
#include "../pipeline/CalenhadModel.h"
#include "../pipeline/TileProducer.h"

using namespace qtwebapp;


enum CalenhadRequestPart { Service = 1, Format = 2, ModuleName = 3, TileX = 4, TileY = 5, TileZ = 6 };

class CalenhadRequestHandler : public HttpRequestHandler {
public:
    CalenhadRequestHandler (QObject* parent);
    virtual ~CalenhadRequestHandler();
    void service (HttpRequest &request, HttpResponse &response);
    void setModel (CalenhadModel* model);

protected:
    int _size;
    QByteArray* _content = nullptr;
    QBuffer* _buffer = nullptr;
    std::shared_ptr<QImage> _tile = nullptr;
    noise::module::Module* findModule (const QString& name);
    void setTileSize (const unsigned int& s);
    void parseTilePath (const QByteArray& path, QByteArray& message, int& statusCode, QByteArray& statusMessage, QByteArray* content);
    CalenhadModel* _model;

};


#endif //CALENHAD_CALENHADREQUESTHANDLER_H
