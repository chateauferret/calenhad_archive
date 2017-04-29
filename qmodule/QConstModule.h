//
// Created by martin on 27/11/16.
//

#ifndef CALENHAD_QCONSTANTMODULE_H
#define CALENHAD_QCONSTANTMODULE_H



#include <QtCore/QString>
#include <libnoise/module/modulebase.h>
#include "QModule.h"
#include <libnoise/module/const.h>
#include <QtWidgets/QDoubleSpinBox>

using namespace noise::module;
class QConstModule : public QModule {
    Q_OBJECT
public:
    void initialise();
    double constValue ();
    Const* module() override;
    Q_PROPERTY (double constantValue READ constValue WRITE setConstValue());
    static QConstModule* newInstance();
    QConstModule* clone() override;
    QString moduleType() override;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;


public slots:
    void setConstValue (double value);

private:
    QDoubleSpinBox* constValueSpin;
    QConstModule (QWidget* parent = 0);

};


#endif //CALENHAD_QCONSTANTMODULE_H
