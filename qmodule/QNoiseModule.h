//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QNOISEMODULE_H
#define CALENHAD_QNOISEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include "libnoise/module/module.h"
#include <libnoise/module/module.h>

using namespace noise::module;
class QNoiseModule : public QModule {
Q_OBJECT
public:
    static QNoiseModule* newPerlinInstance();
    static QNoiseModule* newBillowInstance();
    static QNoiseModule* newRidgedMultiInstance();
    virtual ~QNoiseModule();
    void initialise() override;
    QNoiseModule* addCopy (CalenhadModel* model)  override;
    QString moduleType () override;
    Q_PROPERTY (double frequency READ frequency WRITE setFrequency);
    Q_PROPERTY (double persistence READ persistence WRITE setPersistence);
    Q_PROPERTY (double lacunarity READ lacunarity WRITE setLacunarity);
    Q_PROPERTY (int octaveCount READ octaveCount WRITE setOctaveCount);
    double frequency();
    double lacunarity();
    double persistence();
    int octaveCount();
    bool hasPersistence();
    virtual void inflate (const QDomElement& element, MessageFactory* messages) override;
    virtual void serialise (QDomDocument& doc, MessageFactory* messages) override;


public slots:
    void setFrequency (double value);
    void setLacunarity (double value);
    void setPersistence (double value);
    void setOctaveCount (int value);

protected:
    QDoubleSpinBox* persistenceSpin = nullptr;
    QDoubleSpinBox* frequencySpin, * lacunaritySpin;
    QSpinBox* octaveSpin;
    QNoiseModule (noise::module::Module* m, QWidget* parent = 0);
    static QNoiseModule* newInstance  (Module* m);

};


#endif //CALENHAD_QNOISEMODULE_H
