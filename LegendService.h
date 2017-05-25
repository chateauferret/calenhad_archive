//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_LEGENDSERVICE_H
#define CALENHAD_LEGENDSERVICE_H


#include <QtCore/QString>

namespace icosphere {
    class Legend;
}

class LegendService {
public:
    virtual icosphere::Legend* find (const QString& name) = 0;
    virtual void provide (const QString& name, icosphere::Legend* legend) = 0;
    virtual void provideFromXml (const QString& fname) = 0;
    virtual void dispose (const QString& name) = 0;
};


#endif //CALENHAD_LEGENDSERVICE_H
