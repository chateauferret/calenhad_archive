//
// Created by ferret on 16/10/2020.
//

#ifndef CALENHAD_TECTONICSMODULE_H
#define CALENHAD_TECTONICSMODULE_H

#include <src/grid/Tectonics.h>
#include "Cache.h"

namespace calenhad {
    namespace grid {
        class CubicSphere;
    }
    namespace module {

        class TectonicsModule : public Cache {
        public:
            explicit TectonicsModule (const QString& type = CalenhadServices::preferences() -> calenhad_module_tectonics);
            ~TectonicsModule() override;
            void refresh() override;
            void invalidate() override;
            bool isComplete () override;
        private:
            calenhad::grid::Tectonics* _tectonics;

            void simulate ();
            QAction* _simulateAction;
            bool _simulated;
        };


    }
}


#endif //CALENHAD_TECTONICSMODULE_H
