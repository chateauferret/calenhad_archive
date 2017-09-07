//
// Created by martin on 26/06/17.
//

#ifndef CALENHAD_LEGENDTEST_H
#define CALENHAD_LEGENDTEST_H

//
// Created by martin on 25/06/17.
//

#include <QTest>
#include "../legend/Legend.h"
#include <preferences/preferences.h>
#include "legend/LegendRoster.h"
#include "../CalenhadServices.h"
#include "QTestSuite.h"
namespace calenhad {
    namespace legend {
        class Legend;
    }
}

class LegendTest : public QTestSuite {
Q_OBJECT

public:
    LegendTest();
    ~LegendTest();
    calenhad::legend::Legend* legend;
    calenhad::preferences::Preferences* preferences;
    calenhad::legend::LegendRoster* roster;
    QList<calenhad::legend::LegendEntry> entries;
private slots:

    void initTestCase();
    void init();
    void cleanUp();
    void cleanUpTestCase();
    void testFindValue();
    void testHighestEntry();
    void testBelowMinimum();
    void testEntryOnBoundary();

};


#endif //CALENHAD_LEGENDTEST_H

