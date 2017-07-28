//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QNOISEMODULE_H
#define CALENHAD_QNOISEMODULE_H

#include "QModule.h"
#include <QtWidgets/QWidget>
#include <QDoubleSpinBox>

namespace calenhad {
    namespace qmodule {

        class QNoiseModule : public QModule {
        Q_OBJECT
        public:
            static QNoiseModule* newPerlinInstance ();

            static QNoiseModule* newBillowInstance ();

            static QNoiseModule* newRidgedMultiInstance ();

            virtual ~QNoiseModule ();

            void initialise () override;

            QNoiseModule* clone () override;

            QString nodeType () override;

            double getFrequency ();

            double getLacunarity ();

            double getPersistence ();

            int getOctaveCount ();

            bool hasPersistence ();

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;


            // properties
            Q_PROPERTY (double frequency
                                READ
                                        getFrequency
                                WRITE
                                setFrequency);
            Q_PROPERTY (double lacunarity
                                READ
                                        getLacunarity
                                WRITE
                                setLacunarity);
            Q_PROPERTY (double persistence
                                READ
                                        getPersistence
                                WRITE
                                setPersistence);
            Q_PROPERTY (int octaveCount
                                READ
                                        getOctaveCount
                                WRITE
                                setOctaveCount);


        public slots:

            void setFrequency (const double& value);

            void setLacunarity (const double& value);

            void setPersistence (const double& value);

            void setOctaveCount (const int& value);

        protected:
            QDoubleSpinBox* persistenceSpin, * frequencySpin, * lacunaritySpin;
            QSpinBox* octaveSpin;

            QNoiseModule (noise::module::Module* m, QWidget* parent = 0);

            static QNoiseModule* newInstance (noise::module::Module* m);

        };
    }
}

#endif //CALENHAD_QNOISEMODULE_H
