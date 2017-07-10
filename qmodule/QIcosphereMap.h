//
// Created by martin on 06/03/17.
//

#ifndef CALENHAD_QICOSPHERENODE_H
#define CALENHAD_QICOSPHERENODE_H


#include "QModule.h"
#include "../icosphere/Bounds.h"
#include <memory>
#include <QPushButton>
#include <QLabel>
#include <icosphere/icosphere.h>
#include <libnoiseutils/IcosphereModule.h>

namespace icosphere {
    class Icosphere;
}


class QModule;

using namespace noise::module;

class QIcosphereMap : public QModule {
    Q_OBJECT
public:
    static QIcosphereMap* newInstance();
    virtual ~QIcosphereMap();
    void initialise() override;
    bool isRenderable() override;
    IcosphereModule* module() override;
    QIcosphereMap* clone () override;
    QString nodeType () override;
    void setIcosphereDepth (const unsigned& depth);
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

    //void icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere);

public slots:
    void setIcosphereDepth();
    void setBounds (const icosphere::Bounds&);
    virtual bool generateMap() override;
    //void buildIcosphere ();

signals:
    void complete (std::shared_ptr<icosphere::Icosphere>);
    void icosphereChangeRequested();
    void progress (int done);

protected:
    QIcosphereMap (QWidget* parent = 0);
    unsigned _depth;
    icosphere::Bounds _bounds;
    QSpinBox* _depthSpin;
    QLabel* _vertexCountLabel;
    virtual void addInputPorts() override;
    const QString _key = "altitude";

};


#endif //CALENHAD_QICOSPHERENODE_H
