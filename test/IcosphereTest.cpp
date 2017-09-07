 //
// Created by martin on 26/06/17.
//

#include <CalenhadServices.h>
#include <random>
#include <QtTest/qbenchmark.h>
#include <QtTest/qtestcase.h>
#include <QtTest/qtestdata.h>
#include <memory>
#include <QtTest/QTest>
#include <icosphere/IcosphereModule.h>
#include <libnoise/module/perlin.h>
#include "IcosphereTest.h"
#include "icosphere/icosphere.h"
#include "../icosphere/icosphere.h"
#include "icosphere/IcosphereBuilder.h"

using namespace icosphere;
using namespace geoutils;
using namespace noise::utils;
using namespace noise::module;
using namespace icosphere::test;
using namespace calenhad;
using namespace calenhad::legend;
using namespace calenhad::preferences;
 using namespace noise::module;

IcosphereTest::IcosphereTest() {

}

IcosphereTest::~IcosphereTest() {

}

void IcosphereTest::initTestCase() {
    preferences = new Preferences ();
    preferences->loadSettings ();
    CalenhadServices::providePreferences (preferences);

    roster = new LegendRoster ();
    CalenhadServices::provideLegends (roster);

    legend = new Legend();
    legend -> setInterpolated (false);
    QList<LegendEntry> entries;
    entries.append (LegendEntry (-2.0, QColor (Qt::black)));
    entries.append (LegendEntry (-0.8, QColor (Qt::red)));
    entries.append (LegendEntry (-0.6, QColor (Qt::green)));
    entries.append (LegendEntry (-0.4, QColor (Qt::yellow)));
    entries.append (LegendEntry (-0.2, QColor (Qt::cyan)));
    entries.append (LegendEntry (0.0, QColor (Qt::magenta)));
    entries.append (LegendEntry (0.2, QColor (Qt::gray)));
    entries.append (LegendEntry (0.4, QColor (Qt::darkBlue)));
    entries.append (LegendEntry (0.6, QColor (Qt::darkRed)));
    entries.append (LegendEntry (0.8, QColor (Qt::darkGreen)));
    entries.append (LegendEntry (2.0, QColor (Qt::white)));
    legend -> setName ("default");
    legend -> setEntries (entries);
    roster -> provide (legend);
    _icosphere = std::make_shared<Icosphere> (7);
    _icosphere->assemble (Bounds());

}

void IcosphereTest::init() {
    maxdist = 0.0;
    totaldist = 0.0;
    seed = time (NULL);
    e = std::default_random_engine (seed);
    random = std::uniform_real_distribution<double> (0.0, 1.0);
    timeStart = clock() / static_cast<double> (CLOCKS_PER_SEC);
}

void IcosphereTest::cleanUp() {

}

void IcosphereTest::cleanUpTestCase() {
    roster -> remove ("legend");
    delete legend;
    delete preferences;
    delete roster;
}


void IcosphereTest::buildSpeed() {
    for (int i = 2; i != 7; i++) {
        //QBENCHMARK {
            Icosphere* ico = new Icosphere (i);
        ico->assemble (Bounds());
            delete ico;
        //}
    }
}

void IcosphereTest::imageSpeed() {
    /*
    std::shared_ptr<Icosphere> ico = std::make_shared<Icosphere> (8);

    Perlin* perlin = new Perlin ();
    IcosphereModule* module = new IcosphereModule();
    module -> SetSourceModule (0, *perlin);
    IcosphereBuilder* builder = new IcosphereBuilder (ico);

    builder->setDepth (8);
    builder->setBounds (Bounds());
    builder -> setKey ("altitude");
    builder -> setModule (perlin);
    builder->build ();
    builder -> fill();


    QBENCHMARK {
        QImage* image = new QImage (256, 256, QImage::Format_RGB888);
        Geolocation nw (30, 30, Units::Degrees);
        Geolocation se (60, 0, Units::Degrees);
        int i;
        for (i = 0; i < 10; i++) {
//            ico -> getImage (image, legend, Bounds (se.latitude, nw.latitude, se.longitude, se.latitude), "altitude");
        }
        delete image;
    }
     */
}

void IcosphereTest::searchTimes() {
    QBENCHMARK {
        std::default_random_engine e = std::default_random_engine (seed);
        std::uniform_real_distribution<double> random = std::uniform_real_distribution<double> (0.0, 1.0);
        double timeStart = clock() / static_cast<double> (CLOCKS_PER_SEC);
        double maxdist = 0.0, totaldist = 0.0;
        unsigned points;
        for (points = 0; points < 100; points++) {
            double lat = (random (e) - 0.5) * M_PI;
            double lon = (random (e) - 0.5) * M_PI * 2;
            Geolocation sought = Geolocation (lat, lon, Units::Radians);
            Vertex* found = _icosphere->nearest (sought);
            double dist = _icosphere->distance (found->getGeolocation (), sought);
            maxdist = dist > maxdist ? dist : maxdist;
            totaldist += dist;
        }
        qDebug() << "Average distance from target: " << totaldist / points;
        QVERIFY (totaldist / points < maxdist * 2);
        QVERIFY (totaldist / points < 0.025);
    }
}


// Exercising class Bounds and its use

void IcosphereTest::icosphereBoundsTest_data() {
    QTest::addColumn<double> ("n");
    QTest::addColumn<double> ("s");
    QTest::addColumn<double> ("e");
    QTest::addColumn<double> ("w");
    QTest::addColumn<int> ("count");
    QTest::addColumn<bool> ("crossesDateline");

    QTest::newRow ("Whole globe") << 90.0 << -90.0 << 180.0 << -180.0 << 163842 << true;
    QTest::newRow ("30 deg square") << 30.0 << 0.0 << 30.0 << 0.0 << 4613 << false;
    QTest::newRow ("60 deg square") << 60.0 << 0.0 << 60.0 << 0.0 << 13693 << false;
    QTest::newRow ("Across equator") << 30.0 << -30.0 << 30.0 << -30.0 << 15175 << false;
    QTest::newRow ("Not across dateline") << 30.0 << 0.0 << 150.0 << -150.0 << 38944 << false;
    QTest::newRow ("Across dateline") <<  30.0 << 0.0 << -150.0 << 150.0 << 7180 << true;
    //QTest::newRow ("Across dateline and pole") << 90.0 << 30.0 << -150.0 << 150.0 << 7752 << true;
    QTest::newRow ("Across pole") << 90.0 << 30.0 << 60.0 << 0.0 << 7752 << false;

}

void IcosphereTest::icosphereBoundsTest() {

    QFETCH (double, n);
    QFETCH (double, s);
    QFETCH (double, e);
    QFETCH (double, w);
    QFETCH (int, count);
    QFETCH (bool, crossesDateline);

    icosphere::Bounds bounds (degreesToRadians (n), degreesToRadians (s), degreesToRadians (e), degreesToRadians (w));
    Icosphere* ico = new Icosphere (8);
    ico->assemble (bounds);
    qDebug() << "Estimate: " << bounds.estimateVertexCount (8);
    QCOMPARE (bounds.crossesDateline(), crossesDateline);
    QCOMPARE (ico -> vertexCount(), (unsigned) count);
    delete ico;

}


// Exercising class IcosphereBuilder


void IcosphereTest::icosphereBuilderTest_data() {
    QTest::addColumn<double> ("n");
    QTest::addColumn<double> ("s");
    QTest::addColumn<double> ("w");
    QTest::addColumn<double> ("e");

    QTest::newRow ("Whole globe") << 90.0 << -90.0 << 180.0 << -180.0;
    QTest::newRow ("30 deg square") << 30.0 << 0.0 << 30.0 << 0.0;
    QTest::newRow ("60 deg square") << 60.0 << 0.0 << 60.0 << 0.0;
    QTest::newRow ("Across equator") << 30.0 << -30.0 << 30.0 << -30.0;
    QTest::newRow ("Not across dateline") << 30.0 << 0.0 << 150.0 << -150.0;
    QTest::newRow ("Across dateline") <<  30.0 << 0.0 << -150.0 << 150.0; // across dateline
    QTest::newRow ("Across dateline and both poles") << 90.0 << 0.0 << -150.0 << 150.0; // across dateline
}

void IcosphereTest::icosphereBuilderTest() {

    QFETCH (double, n);
    QFETCH (double, s);
    QFETCH (double, e);
    QFETCH (double, w);
    std::shared_ptr<Icosphere> ico = std::make_shared<Icosphere> (8);
    Perlin* perlin = new Perlin ();
    IcosphereModule* module = new IcosphereModule();
    module -> SetSourceModule (0, *perlin);
    IcosphereBuilder* builder = new IcosphereBuilder (ico);

    builder->setDepth (8);
    builder->setBounds (Bounds (degreesToRadians (n), degreesToRadians (s), degreesToRadians (e), degreesToRadians (w)));
    builder -> setKey ("altitude");
    builder -> setModule (perlin);
    builder->build();
    builder -> fill();


    for (int i = 1000; i < ico -> vertexCount(); i += 100) {
        Vertex* v = (*ico) [i];
        Cartesian c = v -> getCartesian();
        Vertex* found = ico -> nearest (Math::toGeolocation (c));
        double foundValue = found -> getDatum ("altitude");
        QCOMPARE (v -> getId (), found -> getId ());
        QVERIFY (std::abs (Math::toGeolocation (c).latDegrees - found -> getGeolocation ().latDegrees) < 0.00001);
        QVERIFY (std::abs (Math::toGeolocation (c).lonDegrees - found -> getGeolocation ().lonDegrees) < 0.00001);
        QCOMPARE (foundValue, perlin->GetValue (c.x, c.y, c.z));
    }
    delete perlin;
    delete builder;
}


static IcosphereTest instance;

#include "IcosphereTest.moc"