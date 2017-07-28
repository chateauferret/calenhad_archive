//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QROTATEMODULE_H
#define CALENHAD_QROTATEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>
#include <qwt/qwt_dial.h>

namespace calenhad {
    namespace qmodule {


        using namespace noise::module;

        class QRotateModule : public QModule {
        Q_OBJECT
        public:
            static QRotateModule* newInstance ();

            virtual ~QRotateModule ();

            void initialise () override;

            Q_PROPERTY (double xAngle
                                READ
                                        xAngle
                                WRITE
                                setXAngle);
            Q_PROPERTY (double yAngle
                                READ
                                        yAngle
                                WRITE
                                setYAngle);
            Q_PROPERTY (double zAngle
                                READ
                                        zAngle
                                WRITE
                                setZAngle);

            double xAngle ();

            double yAngle ();

            double zAngle ();

            QRotateModule* clone () override;

            RotatePoint* module () override;

            QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

        public slots:

            void setXAngle (double value);

            void setYAngle (double value);

            void setZAngle (double value);

        protected:
            QRotateModule (noise::module::RotatePoint* m, QWidget* parent = 0);

            QwtCounter* xAngleSpin, * yAngleSpin, * zAngleSpin;


        };
    }
}


#endif //CALENHAD_QROTATEMODULE_H
