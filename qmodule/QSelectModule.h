//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSELECTMODULE_H
#define CALENHAD_QSELECTMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include "QRangeModule.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QSelectModule : public QRangeModule {
Q_OBJECT
public:
    static QSelectModule* newInstance();
    virtual ~QSelectModule();
    void initialise() override;
    Q_PROPERTY (double falloff READ falloff WRITE setFalloff);
    double falloff();
    double lowerBound();
    double upperBound();

    Select* module () override;
    QSelectModule* addCopy (CalenhadModel* model) override;
    virtual QString moduleType() override;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

public slots:
    void setFalloff (double value);

protected:
    QSelectModule (QWidget* parent = 0);
    QDoubleSpinBox* falloffSpin;


    void setBounds (double lowerBound, double upperBound) override;
};


#endif //CALENHAD_QSELECTMODULE_H
