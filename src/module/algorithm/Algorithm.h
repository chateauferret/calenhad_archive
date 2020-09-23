//
// Created by ferret on 17/09/2020.
//

#ifndef MESSAGES_ALGORITHM_H
#define MESSAGES_ALGORITHM_H


#include <src/grid/CubicSphere.h>
namespace calenhad {
    namespace module {
        namespace algorithm {
            class Algorithm {

            public:
                Algorithm() { };
                virtual ~Algorithm() { };
                virtual bool execute (calenhad::grid::CubicSphere* buffer) = 0;
            };

        }
    }
}


#endif //MESSAGES_ALGORITHM_H
