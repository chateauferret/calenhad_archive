//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_LEGENDROSTER_H
#define CALENHAD_LEGENDROSTER_H


#include "LegendService.h"
#include "mapping/Legend.h"
#include <QMap>

class LegendRoster : public LegendService {

public:
    LegendRoster();
    ~LegendRoster();
    Legend* find (const QString& name) override;
    bool exists (const QString& name) override;
    void provide (const QString& name, Legend* legend) override;
    void provideFromXml (const QString& fname) override;
    void dispose (const QString& name) override;
    QMap<QString, Legend*> all();
    void rename (const QString& from, const QString& to) override;
private:
    QMap<QString, Legend*> _legends;

};

#endif //CALENHAD_LEGENDROSTER_H
