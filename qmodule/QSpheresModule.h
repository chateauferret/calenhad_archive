//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSPHERESMODULE_H
#define CALENHAD_QSPHERESMODULE_H

#include "QModule.h"
#include <QtWidgets/QWidget>
#include <libnoise/module/spheres.h>
#include <QtWidgets/QDoubleSpinBox>


namespace calenhad {
    namespace qmodule {
        class QSpheresModule : public QModule {
        Q_OBJECT
        public:
            static QSpheresModule* newInstance ();

            virtual ~QSpheresModule ();

            void initialise () override;

            Q_PROPERTY (double frequency
                                READ
                                        frequency
                                WRITE
                                setFrequency);

            double frequency ();

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

            noise::module::Spheres* module() override;

            QString nodeType () override;

            QSpheresModule* clone () override;

        public slots:

            void setFrequency (double value);

        protected:
            QDoubleSpinBox* frequencySpin;

            QSpheresModule (QWidget* parent = 0);

        };
    }
}


#endif //CALENHAD_QSPHERESMODULE_H
