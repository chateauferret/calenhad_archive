//
// Created by martin on 23/05/17.
//

#ifndef CALENHAD_SCANLINERENDERER_H
#define CALENHAD_SCANLINERENDERER_H


#include <libnoise/noise.h>
#include <QtCore/QMutex>
#include <QtCore/QRunnable>
#include "../mapping/Legend.h"
#include "GlobeRenderJob.h"

namespace noise {
    namespace model {
        class Sphere;
    }
}

class Legend;

class ScanLineRenderer : public QObject, public QRunnable {
    Q_OBJECT
public:
    ScanLineRenderer (const RenderBuffer::iterator& index, noise::model::Sphere* sphere, Legend* legend);
    //ScanLineRenderer (const ScanLineRendererParams& params, noise::model::Sphere* sphere, Legend* legend);
    virtual ~ScanLineRenderer();
    void setFinalScanLine (const bool&);

public slots:
    void run() override;

signals:
    void scanline();
    void complete();


protected:
    bool _final;
    noise::model::Sphere* _sphere;
    Legend* _legend;
    QMutex _mutex;

    RenderLine::iterator _item;
    RenderLine::iterator _end;
};


#endif //CALENHAD_SCANLINERENDERER_H
