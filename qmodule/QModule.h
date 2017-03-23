//
// Created by martin on 16/11/16.
//

#ifndef CALENHAD_QMODULE_H
#define CALENHAD_QMODULE_H


#include <libnoise/module/modulebase.h>
#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QTextEdit>
#include <QUuid>
#include "../nodeedit/qneport.h"
#include "QNode.h"
#include "../controls/QNoiseMapViewer.h"

namespace Marble {
    class GeoDataLatLonAltBox;
}

enum ModuleType {
    NONE,
    CYLINDERS,
    SPHERES,
    EXPONENT,
    TRANSLATE,
    ROTATE,
    SCALEPOINT,
    CLAMP,
    CONSTANT,
    ADD,
    ABS,
    BLEND,
    CACHE,
    CHECKERBOARD,
    INVERT,
    MAX,
    MIN,
    MULTIPLY,
    POWER,
    DISPLACE,
    DIFF,
    PERLIN,
    BILLOW,
    RIDGEDMULTI,
    SCALEBIAS,
    SELECT,
    TURBULENCE,
    VORONOI,
    ICOSPHEREMAP
};

Q_DECLARE_METATYPE (ModuleType)
using namespace noise::module;

class QModule : public QNode {
Q_OBJECT
public:
    QModule (noise::module::Module* m, QWidget* parent = 0);

    virtual ~QModule();

    void setUniqueName() override;
    // don't want a copy constructor because subclass implementations will have to call initialise()
    virtual QModule* addCopy (CalenhadModel* model) = 0;
    virtual ModuleType type() = 0;
    virtual QString typeString() override = 0;
    static int seed;
    static noise::NoiseQuality noiseQuality;
    QUuid id();
    virtual Module* module();

    public slots:
    void changeBounds (const GeoDataLatLonBox&);

signals:
    void initialised();

protected:

    virtual void addInputPorts();
    void initialise();
    QFormLayout* _previewLayout;
    QNoiseMapViewer* _preview;
    QUuid _id;
    noise::module::Module* _module;
};


#endif //CALENHAD_QMODULE_H
