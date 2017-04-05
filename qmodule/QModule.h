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

using namespace noise::module;

class QNEBlockHandle;
class MessageFactory;

class QModule : public QNode {
Q_OBJECT
    Q_ENUMS (ModuleType)
public:
    QModule (noise::module::Module* m, QWidget* parent = 0);

    virtual ~QModule();

    void setUniqueName() override;
    // don't want a copy constructor because subclass implementations will have to call initialise()
    virtual QModule* addCopy (CalenhadModel* model) = 0;
    virtual void inflate (const QDomElement& element, MessageFactory* messages) override;
    virtual void serialise (QDomDocument& doc, MessageFactory* messages) override;
    virtual QString moduleType () = 0;
    static int seed;
    static noise::NoiseQuality noiseQuality;
    virtual Module* module();
    void setHandle (QNEBlockHandle* h);
    void setModel (CalenhadModel* model) override;
    QNEBlockHandle* handle();

    public slots:
    void changeBounds (const GeoDataLatLonBox&);
    void refresh();
    void invalidate() override;

signals:
    void initialised();

protected:

    virtual void addInputPorts();
    void initialise() override;
    QFormLayout* _previewLayout;
    QNoiseMapViewer* _preview;
    noise::module::Module* _module;
    QNEBlockHandle* _handle;
    int _previewIndex;



    bool _renderRequested;
};


#endif //CALENHAD_QMODULE_H
