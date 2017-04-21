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
    double getFrequency ();
    double getLacunarity ();
    double getPersistence ();
    int getOctaveCount ();
    bool hasPersistence();
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;


    // properties
    Q_PROPERTY (double frequency READ getFrequency WRITE setFrequency NOTIFY frequencyChanged);
    Q_PROPERTY (double lacunarity READ getLacunarity WRITE setLacunarity NOTIFY lacunarityChanged);
    Q_PROPERTY (double persistence READ getPersistence WRITE setPersistence NOTIFY persistenceChanged);
    Q_PROPERTY (int octaveCount READ getOctaveCount WRITE setOctaveCount NOTIFY octaveCountChanged);


public slots:
    void frequencyChangeRequested (const double& value);
    void lacunarityChangeRequested (const double& value);
    void persistenceChangeRequested (const double& value);
    void octaveCountChangeRequested (const int& value);


    void setFrequency (const double& value);
    void setLacunarity (const double& value);
    void setPersistence (const double& value);
    void setOctaveCount (const int& value);

signals:
    void frequencyChanged (const double&);
    void lacunarityChanged (const double&);
    void persistenceChanged (const double&);
    void octaveCountChanged (const int&);

protected:
    QDoubleSpinBox* persistenceSpin = nullptr;
    QDoubleSpinBox* frequencySpin, * lacunaritySpin;
    QSpinBox* octaveSpin;
    QNoiseModule (noise::module::Module* m, QWidget* parent = 0);
    static QNoiseModule* newInstance  (Module* m);

};


#endif //CALENHAD_QNOISEMODULE_H
