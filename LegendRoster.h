//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_LEGENDROSTER_H
#define CALENHAD_LEGENDROSTER_H


#include "LegendService.h"
#include "icosphere/legend.h"
#include <QMap>

class LegendRoster : public LegendService {

public:
    LegendRoster();
    ~LegendRoster();
    icosphere::Legend* find (const QString& name) override;
    void provide (const QString& name, icosphere::Legend* legend) override;
    void provideFromXml (const QString& fname) override;
    void dispose (const QString& name) override;

private:
    QMap<QString, icosphere::Legend*> _legends;

};

#endif //CALENHAD_LEGENDROSTER_H
