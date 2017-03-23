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
    QBasicModule* addCopy (CalenhadModel* model) override;
    ModuleType type() override;
    //void inflate (const QDomElement& element);
    //QDomElement serialise();
    void initialise();
    static QBasicModule* newInstance (const ModuleType& type);
private:
    QBasicModule (Module* m, QWidget* parent = 0);
    QString typeString() override;
};


#endif //CALENHAD_QBASICMODULE_H
