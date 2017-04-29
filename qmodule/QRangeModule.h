//
// Created by martin on 19/12/16.
//

#ifndef CALENHAD_QBOUNDEDMODULE_H
#define CALENHAD_QBOUNDEDMODULE_H

#include "QModule.h"

class QRangeModule : public QModule {
Q_OBJECT

public:

    virtual void initialise();
    virtual double lowerBound() = 0;
    virtual double upperBound() = 0;
    virtual Module* module() override = 0;
    virtual QString moduleType () override = 0;
    virtual QRangeModule* clone() override = 0;
    Q_PROPERTY (double lowerBound READ lowerBound WRITE setLowerBound);
    Q_PROPERTY (double upperBound READ upperBound WRITE setUpperBound);
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

public slots:
    void setLowerBound (double lowerBound);
    void setUpperBound (double upperBound);

protected:
    QDoubleSpinBox* upperBoundSpin;
    QDoubleSpinBox* lowerBoundSpin;
    QRangeModule (Module* module, QWidget* parent = 0);
    virtual void setBounds (double lowerBound, double upperBound) = 0;
};


#endif //CALENHAD_QBOUNDEDMODULE_H
