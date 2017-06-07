//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QCOMBINERMODULE_H
#define CALENHAD_QCOMBINERMODULE_H


#include <QtCore/QString>
#include <libnoise/module/modulebase.h>
#include "QModule.h"
#include "QRangeModule.h"
#include <libnoise/module/clamp.h>

using namespace noise::module;
class QClampModule : public QRangeModule {
    Q_OBJECT
public:
    static QClampModule* newInstance();
    void initialise() override;
    Clamp* module() override;
    QString nodeType () override;
    double upperBound() override;
    double lowerBound() override;
    QClampModule* clone() override;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

private:
    QClampModule (QWidget* parent = 0);
    void setBounds (double lowerBound, double upperBound) override;
};


#endif //CALENHAD_QCOMBINERMODULE_H
