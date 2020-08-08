//
// Created by ferret on 07/08/2020.
//

#ifndef MESSAGES_CACHE_H
#define MESSAGES_CACHE_H

#include "Convolution.h"

namespace calenhad {
    namespace module {
        class Cache : public Convolution {
            explicit Cache (const QString& nodeType);
            virtual ~Cache();


            void invalidate();

            void refresh();
        };
    }
}

#endif //MESSAGES_CACHE_H
