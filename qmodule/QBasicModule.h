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
    virtual void inflate (const QDomElement& element, MessageFactory* messages) override;
    virtual void serialise (QDomDocument& doc, MessageFactory* messages) override;
    void initialise();
    QString moduleType () override;
    static QBasicModule* newInstance (const QString& type);
private:
    QBasicModule (Module* m, QWidget* parent = 0);
};


#endif //CALENHAD_QBASICMODULE_H
