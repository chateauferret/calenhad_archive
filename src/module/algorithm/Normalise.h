//
// Created by ferret on 17/09/2020.
//

#ifndef MESSAGES_NORMALISE_H
#define MESSAGES_NORMALISE_H
#include "Algorithm.h"


namespace calenhad {
    namespace module {
        namespace algorithm {
            class Normalise : public Algorithm {

            public:
                Normalise();
                virtual ~Normalise();
                bool execute (calenhad::grid::CubicSphere* buffer) override;
            };

        }
    }
}


#endif //MESSAGES_NORMALISE_H
