//
// Created by ferret on 07/02/2021.
//

#ifndef CALENHAD_FRACTALGRID_H
#define CALENHAD_FRACTALGRID_H
#include "icosphere.h"

namespace calenhad {
    namespace grid {

        class FractalGrid : public Icosphere {

        public:
            explicit FractalGrid (const int& depth, const int& sampleDepth, const int& seed, CubicSphere* buffer);

            void setValue (Vertex* v) override;

        protected:
            int _sampleDepth;
            const int& _seed;
            CubicSphere* _buffer;
        };

    }
}


#endif //CALENHAD_FRACTALGRID_H
