//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_LEGENDSERVICE_H
#define CALENHAD_LEGENDSERVICE_H


#include <QtCore/QString>

class Legend;

class LegendService {
public:
    virtual Legend* find (const QString& name) = 0;
    virtual bool exists (const QString& name) = 0;
    virtual void provide (Legend* legend) = 0;
    virtual void inflate (const QString& fname) = 0;
    virtual QMap<QString, Legend*> all() = 0;
    virtual void rename (const QString& from, const QString& to) = 0;
    virtual int legendCount() = 0;
    virtual void remove (const QString& name) = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual Legend* defaultLegend() = 0;
};


#endif //CALENHAD_LEGENDSERVICE_H
