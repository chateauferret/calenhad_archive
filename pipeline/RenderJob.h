//
// Created by martin on 19/05/17.
//

#ifndef CALENHAD_RENDERJOB_H
#define CALENHAD_RENDERJOB_H


#include <QtCore/QObject>
#include <libnoise/module/modulebase.h>
#include "../mapping/Legend.h"
#include <marble/GeoDataLatLonBox.h>
#include <memory>
#include <QtCore/QMutex>
#include <qmodule/QModule.h>

class RenderJob : public QObject {
Q_OBJECT
public:
    RenderJob (const Marble::GeoDataLatLonBox& box, QModule* module, Legend* legend);
    virtual ~RenderJob ();
    virtual bool canRender (const Module* module);
    virtual void render () = 0;
    bool isAbandoned();

public slots:
    virtual void startJob() = 0;
    void abandon();


signals:
    void progress (const double&);
    void abandoned();


protected:
    Marble::GeoDataLatLonBox _bounds;
    QModule* _module;
    int _percentComplete;
    QMutex _abandonMutex;
    Legend* _legend;
    bool _abandoned;

};


#endif //CALENHAD_RENDERJOB_H
