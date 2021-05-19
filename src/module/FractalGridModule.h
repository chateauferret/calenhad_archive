//
// Created by ferret on 07/02/2021.
//

#ifndef CALENHAD_FRACTALGRIDMODULE_H
#define CALENHAD_FRACTALGRIDMODULE_H

#include "Cache.h"

namespace calenhad {
    namespace grid {
        class CubicSphere;
        class FractalGrid;
    }
    namespace module {

        class FractalGridModule : public Cache {
            Q_OBJECT
        public:
            explicit FractalGridModule (const QString& type = CalenhadServices::preferences() -> calenhad_module_fractalgrid);
            ~FractalGridModule() override;
            void refresh() override;
            void invalidate() override;
            bool isComplete () override;
        private:
            calenhad::grid::FractalGrid* _grid;
            QLabel* _sampleLevelVerticesLabel, * _generateLevelVerticesLabel;

            void generate ();
            QAction* _generateAction;
            bool _generated;
            int _sampleLevel, _generateLevel;


            QSpinBox* generateLevelBox;
            QSpinBox* sampleLevelBox;
        };


    }
}



#endif //CALENHAD_FRACTALGRIDMODULE_H
