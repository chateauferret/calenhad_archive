//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QTRANSLATEMODULE_H
#define CALENHAD_QTRANSLATEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QTranslateModule : public QModule {
Q_OBJECT
public:
    static QTranslateModule* newInstance();
    virtual ~QTranslateModule();
    void initialise() override;
    Q_PROPERTY (double dX READ dX WRITE setDX);
    Q_PROPERTY (double dY READ dY WRITE setDY);
    Q_PROPERTY (double dZ READ dZ WRITE setDZ);
    double dX();
    double dY();
    double dZ();

    TranslatePoint* module () override;
    QTranslateModule* clone () override;
    virtual QString nodeType () override;

    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

public slots:
    void setDX (double value);
    void setDY (double value);
    void setDZ (double value);

protected:
    QTranslateModule (QWidget* parent = 0);
    QDoubleSpinBox* dXSpin, * dYSpin, * dZSpin;
};


#endif //CALENHAD_QTRANSLAtEMODULE_H
