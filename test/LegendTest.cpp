//
// Created by martin on 25/06/17.
//
#include "LegendTest.h"

using namespace calenhad::preferences;
using namespace calenhad;
using namespace calenhad::legend;
    LegendTest::LegendTest() {


    }

    LegendTest::~LegendTest() {

    }

    void LegendTest::initTestCase() {

    }

    void LegendTest::init() {
        preferences = new Preferences ();
        preferences->loadSettings ();
        CalenhadServices::providePreferences (preferences);

        roster = new LegendRoster ();
        CalenhadServices::provideLegends (roster);

        legend = new Legend();
        legend -> setInterpolated (false);
        QList<LegendEntry> entries;
        entries.append (LegendEntry (-1.0, QColor (Qt::black)));
        entries.append (LegendEntry (-0.8, QColor (Qt::red)));
        entries.append (LegendEntry (-0.6, QColor (Qt::green)));
        entries.append (LegendEntry (-0.4, QColor (Qt::yellow)));
        entries.append (LegendEntry (-0.2, QColor (Qt::cyan)));
        entries.append (LegendEntry (0.0, QColor (Qt::magenta)));
        entries.append (LegendEntry (0.2, QColor (Qt::gray)));
        entries.append (LegendEntry (0.4, QColor (Qt::darkBlue)));
        entries.append (LegendEntry (0.6, QColor (Qt::darkRed)));
        entries.append (LegendEntry (0.8, QColor (Qt::darkGreen)));
        entries.append (LegendEntry (1.0, QColor (Qt::white)));
        legend -> setName ("default");
        legend -> setEntries (entries);
        roster -> provide (legend);

    }

    void LegendTest::cleanUp() {
        roster -> remove ("legend");
        delete legend;
        delete preferences;
        delete roster;
    }

    void LegendTest::cleanUpTestCase() {

    }



    void LegendTest::testFindValue() {
        QCOMPARE (legend -> lookup (0.205), QColor (Qt::gray));
        QCOMPARE (legend -> lookup (0.050), QColor (Qt::magenta));
        QCOMPARE (legend -> lookup (0.450), QColor (Qt::darkBlue));
    }

    void LegendTest::testHighestEntry() {
        QCOMPARE (legend -> lookup (1.1), QColor (Qt::white));
    }

    void LegendTest::testBelowMinimum() {
        QCOMPARE (legend -> lookup (-1.0), QColor (Qt::black));
    }

    void LegendTest::testEntryOnBoundary() {
//        QCOMPARE (legend -> lookup (0.200), QColor (Qt::gray));
    }



static LegendTest instance;

#include "LegendTest.moc"