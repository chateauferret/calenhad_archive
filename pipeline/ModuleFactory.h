//
// Created by martin on 11/11/16.
//

#ifndef CALENHAD_MODULEFACTORY_H
#define CALENHAD_MODULEFACTORY_H


#include <libnoise/module/modulebase.h>
#include <QString>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <libnoise/noise.h>
#include <libnoise/module/abs.h>
#include <QtCore/QSet>
#include "../qmodule/QModule.h"


class ModuleFactory : public QObject {
    Q_OBJECT
public:
    ModuleFactory();
    ~ModuleFactory();
    QModule* createModule (const ModuleType& moduleType, CalenhadModel* model);

    void setSeed (const int& seed);
    void setNoiseQuality (const noise::NoiseQuality& noiseQuality);
    int seed();
    noise::NoiseQuality noiseQuality();
signals:
    void seedChanged (const int& seed);
    void noiseQualityChanged (const noise::NoiseQuality& noiseQuality);

private:
    noise::NoiseQuality _noiseQuality = noise::NoiseQuality::QUALITY_STD;
    int _seed = 0;
    QMap<ModuleType, QModule*> _repository;
    QSet<QModule*> _roster;
    void makeRepository();
};


#endif //CALENHAD_MODULEFACTORY_H
