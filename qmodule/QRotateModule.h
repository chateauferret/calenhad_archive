//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QROTATEMODULE_H
#define CALENHAD_QROTATEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QRotateModule : public QModule {
Q_OBJECT
public:
    static QRotateModule* newInstance();
    virtual ~QRotateModule();
    void initialise() override;
    Q_PROPERTY (double xAngle READ xAngle WRITE setXAngle);
    Q_PROPERTY (double yAngle READ yAngle WRITE setYAngle);
    Q_PROPERTY (double zAngle READ zAngle WRITE setZAngle);
    double xAngle();
    double yAngle();
    double zAngle();
    QRotateModule* addCopy (CalenhadModel* model) override;
    RotatePoint* module () override;
    QString moduleType () override;

    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;

public slots:
    void setXAngle (double value);
    void setYAngle (double value);
    void setZAngle (double value);

protected:
    QRotateModule (noise::module::RotatePoint* m, QWidget* parent = 0);
    QDoubleSpinBox* xAngleSpin, * yAngleSpin, * zAngleSpin;


};


#endif //CALENHAD_QROTATEMODULE_H
