//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QCYLINDERSMODULE_H
#define CALENHAD_QCYLINDERSMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QCylindersModule : public QModule {
Q_OBJECT
public:
    static QCylindersModule* newInstance();
    virtual ~QCylindersModule();
    void initialise() override;
    Q_PROPERTY (double frequency READ frequency WRITE setFrequency);
    double frequency();

    Cylinders* module () override;
    QCylindersModule* clone() override;
    QString nodeType () override;

    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

public slots:
    void setFrequency (double value);

protected:
    QLogSpinBox* frequencySpin;
    QCylindersModule (QWidget* parent = 0);


};


#endif //CALENHAD_QCYLINDERSMODULE_H
