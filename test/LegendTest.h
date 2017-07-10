//
// Created by martin on 26/06/17.
//

#ifndef CALENHAD_LEGENDTEST_H
#define CALENHAD_LEGENDTEST_H

//
// Created by martin on 25/06/17.
//

#include <QTest>
#include <mapping/Legend.h>
#include <preferences.h>
#include "../LegendRoster.h"
#include "../CalenhadServices.h"
#include "QTestSuite.h"

class LegendTest : public QTestSuite {
Q_OBJECT

public:
    LegendTest();
    ~LegendTest();
    Legend* legend;
    Preferences* preferences;
    LegendRoster* roster;
    QList<LegendEntry> entries;
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

