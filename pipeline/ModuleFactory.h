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


namespace calenhad {
    namespace qmodule {
        class QNode;
    }

    namespace pipeline {
        class CalenhadModel;

        class ModuleFactory : public QObject {
        Q_OBJECT

            int getSourceModuleCount (QString moduleType);
            Q_ENUMS (ModuleType)
        public:
            ModuleFactory ();

            ~ModuleFactory ();

            calenhad::qmodule::QNode* createModule (const QString& moduleType);

            void setNoiseQuality (const noise::NoiseQuality& noiseQuality);

            int seed ();

            noise::NoiseQuality noiseQuality ();

            QPixmap* getIcon (const QString& type);

            QStringList types ();

            qmodule::QNode* clone (qmodule::QNode* other);
            QMap<QString, QString>* codes ();
            QStringList paramNames();

        signals:

            void seedChanged (const int& seed);

            void noiseQualityChanged (const noise::NoiseQuality& noiseQuality);

        private:
            noise::NoiseQuality _noiseQuality = noise::NoiseQuality::QUALITY_STD;
            int _seed = 0;
            QMap<QString, QPixmap*> _icons;
            QMap<QString, QString>* _codes;
            QStringList _paramNames;
            void provideCodes ();
            void provideParamNames();


        };
    }
}

#endif //CALENHAD_MODULEFACTORY_H
