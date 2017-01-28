//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSPHERESMODULE_H
#define CALENHAD_QSPHERESMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QSpheresModule : public QModule {
Q_OBJECT
public:
    static QSpheresModule* newInstance();
    virtual ~QSpheresModule();
    void initialise() override;
    Q_PROPERTY (double frequency READ frequency WRITE setFrequency);
    double frequency();

    Spheres* module () override;
    ModuleType type() override;
    QSpheresModule* addCopy (CalenhadModel* model) override;

    public slots:
    void setFrequency (double value);
    virtual QString typeString() override;
protected:
    QDoubleSpinBox* frequencySpin;
    QSpheresModule (QWidget* parent = 0);

};


#endif //CALENHAD_QSPHERESMODULE_H
