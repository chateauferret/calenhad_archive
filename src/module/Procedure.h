//
// Created by ferret on 06/09/2020.
//

#ifndef MESSAGES_PROCEDURE_H
#define MESSAGES_PROCEDURE_H

#include "Cache.h"

namespace calenhad {
    namespace grid {
        class CubicSphere;
    }
    namespace module {


        class Procedure : public Cache {
        public:
            explicit Procedure (const QString& nodeType);
            virtual ~Procedure();
            virtual void refresh();
            int iterations();

            calenhad::grid::CubicSphere* inputBuffer (const int& i);

        protected:
            calenhad::grid::CubicSphere* _buffersIn [4];

        };

    }
}


#endif //MESSAGES_PROCEDURE_H
