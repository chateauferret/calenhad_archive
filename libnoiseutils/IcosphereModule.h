//
// Created by martin on 28/06/17.
//

#ifndef CALENHAD_ICOSPHEREMODULE_H
#define CALENHAD_ICOSPHEREMODULE_H

#include <memory>
#include <QtCore/QString>
#include "libnoise/module/modulebase.h"

namespace icosphere {
    class Icosphere;
    class Bounds;
}

class QProgressNotification;

class IcosphereModule : public QObject, public noise::module::Module {
    Q_OBJECT
public:
    IcosphereModule();
    virtual ~IcosphereModule();
    int vertexCount ();
    int GetSourceModuleCount () const override;
    double GetValue (double x, double y, double z) const override;
    void setKey (const QString& key);
    void buildIcosphere (const icosphere::Bounds& bounds, const int& depth);

protected:
    std::shared_ptr<icosphere::Icosphere> _icosphere;
    QString _key;
    QProgressNotification* _notification;


};


#endif //CALENHAD_ICOSPHEREMODULE_H
