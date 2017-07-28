//
// Created by martin on 07/12/16.
//

#ifndef CALENHAD_QBASICMODULE_H
#define CALENHAD_QBASICMODULE_H
#include "QModule.h"


namespace calenhad {
    namespace qmodule {


        class QBasicModule : public QModule {

        public:
            virtual ~QBasicModule ();

            QBasicModule* clone () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

            void initialise ();

            QString nodeType () override;

            static QBasicModule* newInstance (const QString& type);

            bool hasParameters () override;

        private:
            QBasicModule (noise::module::Module* m, QWidget* parent = 0);
        };
    }
}


#endif //CALENHAD_QBASICMODULE_H
