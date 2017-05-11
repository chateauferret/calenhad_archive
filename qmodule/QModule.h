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

namespace icosphere {
    class Legend;
}

using namespace noise::module;

class QNEBlock;
class MessageFactory;

class QModule : public QNode {
Q_OBJECT
    Q_ENUMS (ModuleType)
public:
    QModule (noise::module::Module* m, QWidget* parent = 0);

    virtual ~QModule();

    void setUniqueName() override;
    // don't want a copy constructor because subclass implementations will have to call syncMouseMode()
    virtual QModule* clone () = 0;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;
    virtual QString moduleType () override = 0;
    static int seed;
    static noise::NoiseQuality noiseQuality;
    virtual noise::module::Module* module();
    void setHandle (QNEBlock* h);
    void setModel (CalenhadModel* model) override;
    QNEBlock* handle();
    void setLegend (icosphere::Legend* legend);
    icosphere::Legend* legend();
    std::shared_ptr<QImage> overview();

public slots:
    void invalidate() override;

protected:

    virtual void addInputPorts();
    void initialise() override;
    QFormLayout* _previewLayout;
    QNoiseMapViewer* _preview;
    noise::module::Module* _module;
    QNEBlock* _handle;
    int _previewIndex;
    icosphere::Legend* _legend;

    bool _renderRequested;

protected slots:
    void setupPreview ();
};


#endif //CALENHAD_QMODULE_H
