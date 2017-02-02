#ifndef DATASET_H
#define DATASET_H
#include <string>
#include "icosphereutils.h"
#include "../libnoiseutils/legend.h"

namespace icosphere {

    class Dataset {
    public:
        Dataset ();

        Dataset (const std::string& n, noise::utils::Legend* l, const char& d);

        ~Dataset ();

        std::string name;
        std::string author;

        void setLegend (noise::utils::Legend* l);

        noise::utils::Legend* getLegend ();

        unsigned char getDepth ();

    protected:
        noise::utils::Legend* legend;
        char depth;

    };
}

#endif // DATASET_H
