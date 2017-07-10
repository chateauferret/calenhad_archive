//
// Created by martin on 26/06/17.
//

#ifndef CALENHAD_ICOSPHERETEST_H
#define CALENHAD_ICOSPHERETEST_H


#include <mapping/Legend.h>
#include <preferences.h>
#include <LegendRoster.h>
#include <random>
#include "QTestSuite.h"
#include <memory>

namespace icosphere {
    class Icosphere;
}

class IcosphereTest : public QTestSuite {
Q_OBJECT
public:

    IcosphereTest();
    ~IcosphereTest();
    Legend* legend;
    Preferences* preferences;
    LegendRoster* roster;
    QList<LegendEntry> entries;
    unsigned points;
    double maxdist, totaldist;
    long timeEnd, timeStart;
    time_t seed;
    std::default_random_engine e;
    std::uniform_real_distribution<double> random;
    std::shared_ptr<icosphere::Icosphere> _icosphere;

private slots:
    void initTestCase();
    void init();
    void cleanUp();
    void cleanUpTestCase();
    void buildSpeed();
    void imageSpeed();
    void searchTimes();
    void icosphereBoundsTest_data();
    void icosphereBoundsTest();
    void icosphereBuilderTest_data();
    void icosphereBuilderTest();

};


#endif //CALENHAD_ICOSPHERETEST_H
