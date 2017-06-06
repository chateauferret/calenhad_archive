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


struct ScanLineRendererParams {
public:
    ScanLineRendererParams (const double& s, const double& n, const double & lon, const double& dLat) : south (s), north (n), longitude (lon), dLatitude (dLat) {

    }
    double south, north, longitude, dLatitude;
};

class ScanLineRenderer : public QObject, public QRunnable {
    Q_OBJECT

public:
    ScanLineRenderer (const ScanLineRendererParams& params, noise::model::Sphere* sphere, Legend* legend);
    virtual ~ScanLineRenderer();
    void setFinalScanLine (const bool&);

public slots:
    void run() override;

signals:
    void scanline (const std::shared_ptr<GlobeBuffer>&);
    void complete (const std::shared_ptr<GlobeBuffer>&);


protected:
    double _lon, _south, _north, _lat, _dLat;
    bool _final;
    noise::model::Sphere* _sphere;
    Legend* _legend;
    QMutex mutex;
    std::shared_ptr<GlobeBuffer> _scanLine;
    void writeRenderPoint (const RenderPoint& point);


};


#endif //CALENHAD_SCANLINERENDERER_H
