//
// Created by ferret on 07/08/2020.
//

#ifndef MESSAGES_CACHE_H
#define MESSAGES_CACHE_H

#include "StructuredGrid.h"

namespace calenhad {
    namespace module {
        class Cache : public StructuredGrid {
        public:
            explicit Cache (const QString& nodeType);
            ~Cache() override;
            void invalidate() override;
            virtual void refresh();
            bool isComputed() override;
            void fetch (grid::CubicSphere* buffer) override;

        protected:
            bool _computed;

        };
    }
}

#endif //MESSAGES_CACHE_H
