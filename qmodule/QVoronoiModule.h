//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QVORONOIMODULE_H
#define CALENHAD_QVORONOIMODULE_H

#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>
#include <QtWidgets/QCheckBox>

using namespace noise::module;
class QVoronoiModule : public QModule {
Q_OBJECT
public:
    static QVoronoiModule* newInstance();
    virtual ~QVoronoiModule();
    void initialise() override;
    Q_PROPERTY (double frequency READ frequency WRITE setFrequency);
    Q_PROPERTY (double displacement READ displacement WRITE setDisplacement);
    Q_PROPERTY (bool enableDistance READ enableDistance WRITE setEnableDistance);
    double frequency();
    double displacement();
    bool enableDistance();
    Voronoi* module() override;
    ModuleType type() override;
    QVoronoiModule* addCopy (CalenhadModel* model) override;
    QVoronoiModule (QWidget* parent = 0);
    QDoubleSpinBox* frequencySpin, * displacementSpin;
    QCheckBox* enableDistanceCheck;
    virtual QString typeString() override;
public slots:
    void setEnableDistance (bool value);
    void setDisplacement (double value);
    void setFrequency (double value);
};


#endif //CALENHAD_QVORONOIMODULE_H
