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

namespace icosphere {
    class Icosphere;
}

namespace noise {
    namespace module {
        class IcosphereMap;
    }
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
    QIcosphereMap* addCopy (CalenhadModel* model) override;
    QString moduleType () override;
    void setIcosphereDepth (const unsigned& depth);
    virtual void inflate (const QDomElement& element, MessageFactory* messages) override;
    virtual void serialise (QDomDocument& doc, MessageFactory* messages) override;
    int icosphereDepth();

public slots:
    void icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere);
    void setIcosphereDepth();
    void setBounds (const icosphere::Bounds&);
    void generateMap();
    void resetUI (const QString&);

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
