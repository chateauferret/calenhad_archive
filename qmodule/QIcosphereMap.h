//
// Created by martin on 06/03/17.
//

#ifndef CALENHAD_QICOSPHERENODE_H
#define CALENHAD_QICOSPHERENODE_H


#include "QModule.h"
#include "../icosphere/icosphere.h"
#include "../icosphere/Bounds.h"
#include <memory>
#include <QPushButton>
#include "../libnoiseutils/icospheremap.h"


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
    void setUniqueName() override;
    void initialise() override;
    bool isRenderable() override;
    IcosphereMap* module() override;
    QIcosphereMap* clone () override;
    QString nodeType () override;
    void setIcosphereDepth (const unsigned& depth);
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;
    int icosphereDepth();

public slots:
    void icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere);
    void setIcosphereDepth();
    void setBounds (const icosphere::Bounds&);
    void generateMap();
    void buildIcosphere ();

signals:
    void icosphereChangeRequested();

protected:
    QIcosphereMap (QWidget* parent = 0);
    unsigned _depth;
    icosphere::Bounds _bounds;
    std::shared_ptr<icosphere::Icosphere> _icosphere;
    QSpinBox* _depthSpin;
    QLabel* _vertexCountLabel;
    virtual void addInputPorts() override;
};


#endif //CALENHAD_QICOSPHERENODE_H
