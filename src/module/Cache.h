//
// Created by ferret on 07/08/2020.
//

#ifndef MESSAGES_CACHE_H
#define MESSAGES_CACHE_H

#include "StructuredGrid.h"
#include "src/module/algorithm/Algorithm.h"

namespace calenhad {
    namespace module {
        class Cache : public StructuredGrid {
        public:
            explicit Cache (const QString& nodeType);
            ~Cache() override;
            void invalidate() override;
            virtual void refresh();
            bool isComputed() override;

            void setAlgorithm (calenhad::module::algorithm::Algorithm* algorithm);

            void fetch (grid::CubicSphere* buffer) override;

        protected:
            calenhad::module::algorithm::Algorithm* _algorithm;
            bool _computed;

        };
    }
}

#endif //MESSAGES_CACHE_H
