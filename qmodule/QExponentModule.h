//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QEXPONENTMODULE_H
#define CALENHAD_QEXPONENTMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QExponentModule : public QModule {
Q_OBJECT
public:
    static QExponentModule* newInstance();
    virtual ~QExponentModule();
    void initialise() override;
    Q_PROPERTY (double exponent READ exponent WRITE setExponent);
    double exponent();

    Exponent* module () override;
    QExponentModule* addCopy (CalenhadModel* model)  override;
    QString moduleType () override;
    virtual void inflate (const QDomElement& element, MessageFactory* messages) override;
    virtual void serialise (QDomDocument& doc, MessageFactory* messages) override;

public slots:
    void setExponent (double value);

protected:
    QExponentModule (QWidget* parent = 0);
    QDoubleSpinBox* exponentSpin;
};


#endif //CALENHAD_QEXPONENTMODULE_H
