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
    void provide (Legend* legend) override;
    void inflate (const QString& filename = _filename) override;
    QMap<QString, Legend*> all();
    void rename (const QString& from, const QString& to) override;
    int legendCount() override;
    void remove (const QString& name) override;
    void commit() override;
    void rollback() override;
private:
    QMap<QString, Legend*> _legends;


    static QString _filename;

    void serialise (QString filename);
};

#endif //CALENHAD_LEGENDROSTER_H
