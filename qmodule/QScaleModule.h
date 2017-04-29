//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSCALEMODULE_H
#define CALENHAD_QSCALEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>
class QScaleModule : public QModule {
Q_OBJECT
public:
    static QScaleModule* newInstance();
    virtual ~QScaleModule();
    void initialise() override;
    Q_PROPERTY (double scaleX READ scaleX WRITE setScaleX);
    Q_PROPERTY (double scaleY READ scaleY WRITE setScaleY);
    Q_PROPERTY (double scaleZ READ scaleZ WRITE setScaleZ);
    double scaleX();
    double scaleY();
    double scaleZ();

    ScalePoint* module () override;
    QScaleModule* clone () override;
    virtual QString moduleType () override;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;


public slots:
    void setScaleX (double value);
    void setScaleY (double value);
    void setScaleZ (double value);

protected:
    QDoubleSpinBox* scaleXSpin, * scaleYSpin, * scaleZSpin;
    QScaleModule (QWidget* parent = 0);
};


#endif //CALENHAD_QSCALEMODULE_H
