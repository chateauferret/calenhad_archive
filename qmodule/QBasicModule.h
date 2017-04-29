//
// Created by martin on 07/12/16.
//

#ifndef CALENHAD_QBASICMODULE_H
#define CALENHAD_QBASICMODULE_H
#include "QModule.h"
#include "../pipeline/ModuleFactory.h"

class QBasicModule : public QModule {

public:
    virtual ~QBasicModule();
    QBasicModule* clone () override;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;
    void initialise();
    QString moduleType () override;
    static QBasicModule* newInstance (const QString& type);
    bool hasParameters() override;
private:
    QBasicModule (Module* m, QWidget* parent = 0);
};


#endif //CALENHAD_QBASICMODULE_H
