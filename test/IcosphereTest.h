//
// Created by martin on 26/06/17.
//

#ifndef CALENHAD_ICOSPHERETEST_H
#define CALENHAD_ICOSPHERETEST_H


#include <legend/Legend.h>
#include <preferences/preferences.h>
#include <legend/LegendRoster.h>
#include <random>
#include "QTestSuite.h"
#include <memory>
namespace calenhad {
    namespace legend {
        class Legend;
    }
}
namespace icosphere {
    class Icosphere;


    namespace test {

        class IcosphereTest : public QTestSuite {
        Q_OBJECT
        public:

            IcosphereTest ();

            ~IcosphereTest ();

            calenhad::legend::Legend* legend;
            calenhad::preferences::Preferences* preferences;
            calenhad::legend::LegendRoster* roster;
            QList<calenhad::legend::LegendEntry> entries;
            unsigned points;
            double maxdist, totaldist;
            long timeEnd, timeStart;
            time_t seed;
            std::default_random_engine e;
            std::uniform_real_distribution<double> random;
            std::shared_ptr<icosphere::Icosphere> _icosphere;

        private slots:

            void initTestCase ();

            void init ();

            void cleanUp ();

            void cleanUpTestCase ();

            void buildSpeed ();

            void imageSpeed ();

            void searchTimes ();

            void icosphereBoundsTest_data ();

            void icosphereBoundsTest ();

            void icosphereBuilderTest_data ();

            void icosphereBuilderTest ();

        };
    }
}


#endif //CALENHAD_ICOSPHERETEST_H
