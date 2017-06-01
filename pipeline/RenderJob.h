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

class RenderJob : public QObject {
Q_OBJECT
public:
    RenderJob (const Marble::GeoDataLatLonBox& box, noise::module::Module* source, Legend* legend);
    virtual ~RenderJob ();
    virtual bool canRender (const noise::module::Module* source);
    virtual void render () = 0;
    bool isAbandoned();

public slots:
    virtual void startJob() = 0;
    void abandon();


signals:
    void progress (const double&);


protected:
    Marble::GeoDataLatLonBox _bounds;
    noise::module::Module* _source;
    int _percentComplete;
    QMutex _abandonMutex;
    Legend* _legend;
    bool _abandoned;


};


#endif //CALENHAD_RENDERJOB_H
