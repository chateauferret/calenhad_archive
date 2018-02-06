//
// Created by martin on 03/09/17.
//

#include "ProjectionTest.h"
#include"../geoutils.h"
#include "../mapping/projection/ProjectionService.h"
#include "../mapping/projection/EquirectangularProjection.h"

using namespace calenhad::mapping::projection;
using namespace calenhad::preferences;
using namespace calenhad;
using namespace geoutils;

ProjectionTest::ProjectionTest() {


}

ProjectionTest::~ProjectionTest() {

}

void ProjectionTest::initTestCase() {

}

void ProjectionTest::init() {

}

void ProjectionTest::cleanUp() {

}

void ProjectionTest::cleanUpTestCase() {

}



void ProjectionTest::equirectangularProjectionInverseTest_data() {
    QTest::addColumn<double> ("lat");
    QTest::addColumn<double> ("lon");
    QTest::addColumn<double> ("x");
    QTest::addColumn<double> ("y");
    QTest::addColumn<bool> ("visible");

    QTest::newRow ("First point of Aries") << 0.0 << 0.0 << 0.0 << 0.0 << true;
    QTest::newRow ("North pole") << 90.0 << 0.0 << 0.0 << 1.0 << true;
    QTest::newRow ("90 deg E, equator") << 0.0 << 90.0 << 1.0 << 0.0 << true;
    QTest::newRow ("90 deg W, equator") << 0.0 << -90.0 << -1.0 << 0.0 << true;

}


void ProjectionTest::equirectangularProjectionInverseTest() {

    QFETCH (double, lat);
    QFETCH (double, lon);
    QFETCH (double, x);
    QFETCH (double, y);
    QFETCH (bool, visible);
    EquirectangularProjection* proj = new EquirectangularProjection ();
    Geolocation g;
    QPointF p;
    // test inverse
    p = QPointF (x, y);
    proj->inverse (p, g);

}


void ProjectionTest::equirectangularProjectionForwardTest_data() {
    QTest::addColumn<double> ("lat");
    QTest::addColumn<double> ("lon");
    QTest::addColumn<double> ("x");
    QTest::addColumn<double> ("y");
    QTest::addColumn<bool> ("visible");

    QTest::newRow ("First point of Aries") << 0.0 << 0.0 << 0.0 << 0.0 << true;
    QTest::newRow ("North pole") << 90.0 << 0.0 << 0.0 << 1.0 << true;
    QTest::newRow ("90 deg E, equator") << 0.0 << 90.0 << 1.0 << 0.0 << true;
    QTest::newRow ("90 deg W, equator") << 0.0 << -90.0 << -1.0 << 0.0 << true;

}

void ProjectionTest::equirectangularProjectionForwardTest() {
    QFETCH (double, lat);
    QFETCH (double, lon);
    QFETCH (double, x);
    QFETCH (double, y);
    QFETCH (bool, visible);
    // test forward
    EquirectangularProjection* proj = new EquirectangularProjection ();
    Geolocation g;
    QPointF p;
    /*
    if  {
        QCOMPARE (p.x (), x);
        QCOMPARE (p.y (), y);
        QCOMPARE (visible, true);
    } else {
        QCOMPARE (p.x (), x);
        QCOMPARE (p.y (), y);
        QCOMPARE (visible, false);
    }
    */
    g = Geolocation (lat, lon, geoutils::Units::Degrees);
    proj  -> forward (g, p);
    delete proj;
}



static ProjectionTest instance;

#include "ProjectionTest.moc"