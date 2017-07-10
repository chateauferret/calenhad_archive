//
// Created by martin on 04/07/17.
//

#ifndef CALENHAD_TECTONICS_H
#define CALENHAD_TECTONICS_H

#include <QtCore/QString>
#include "libnoise/module/modulebase.h"

namespace icosphere {
    class Icosphere;
}

class Tectonics : public noise::module::Module {

public:
    Tectonics();
    virtual ~Tectonics();

    int GetSourceModuleCount () const override;
    double GetValue (double x, double y, double z) const override;

protected:
    icosphere::Icosphere* _icosphere;
    const QString _key = "";
};


#endif //CALENHAD_TECTONICS_H
