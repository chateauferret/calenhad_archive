#include <limits.h>
#include <gtest/gtest.h>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Constants.hpp>
#include <iostream>
#include "../geoutils.h"
#define TOLERANCE 0.00000001

using namespace geoutils;

class GeocentricTestRow {
public:
    double latDegrees, lonDegrees, latRads, lonRads, x, y, z;
    GeocentricTestRow (const double& latd, const double&  lond, const double& latr, const double& lonr, const double& newx, const double& newy, const double& newz) :
        latDegrees (latd), lonDegrees (lond), latRads (latr), lonRads (lonr), x (newx), y (newy), z  (newz) { }
};


class GeocentricTest : public ::testing::TestWithParam<GeocentricTestRow> {
protected:
    virtual void setUpTestCase() {

    }

    virtual void tearDownTestCase() {

    }
};


TEST_P (GeocentricTest, conversion) {

    GeocentricTestRow const& row = GetParam();
    Geolocation g1 (row.latRads, row.lonRads, Units::Radians);
    Cartesian c1 = Math::toCartesian (g1);
    EXPECT_NEAR (row.x, c1.x, TOLERANCE);
    EXPECT_NEAR (row.y, c1.y, TOLERANCE);
    EXPECT_NEAR (row.z, c1.z, TOLERANCE);

    Geolocation g2 (row.latDegrees, row.lonDegrees, Units::Degrees);
    Cartesian c2 = Math::toCartesian (g2);
    EXPECT_NEAR (row.x, c2.x, TOLERANCE);
    EXPECT_NEAR (row.y, c2.y, TOLERANCE);
    EXPECT_NEAR (row.z, c2.z, TOLERANCE);

    Cartesian c3 = Cartesian (row.x, row.y, row.z);
    Geolocation g3 = Math::toGeolocation (c3);
    EXPECT_NEAR (row.latRads, g3.latitude, TOLERANCE);
    EXPECT_NEAR (row.lonRads, g3.longitude, TOLERANCE);
    EXPECT_NEAR (row.latDegrees, g3.latDegrees, TOLERANCE);
    EXPECT_NEAR (row.lonDegrees, g3.lonDegrees, TOLERANCE);

    Cartesian c4 = Math::toCartesian (g3);
    EXPECT_NEAR (c4.x, c3.x, TOLERANCE);
    EXPECT_NEAR (c4.y, c3.y, TOLERANCE);
    EXPECT_NEAR (c4.z, c3.z, TOLERANCE);
}

INSTANTIATE_TEST_CASE_P (conversion, GeocentricTest, ::testing::Values (
    GeocentricTestRow (45, 45, M_PI / 4, M_PI / 4, 1.0/2.0, 1.0/2.0, 1 / sqrt (2) ));
);

class BoundsCrossingDatelineTestRow {
public:
    double _nwLat, _nwLon, _seLat, _seLon, _withinLat, _withinLon;
    bool _crossesDateline, _throwsException;
    BoundsCrossingDatelineTestRow (const double& nwLat, const double& nwLon, const double& seLat, const double& seLon, const double& withinLat, const double& withinLon, const bool& crossesDateline, const bool& throwsException) :
        _nwLat (nwLat), _nwLon (nwLon), _seLat (seLat), _seLon (seLon), _withinLat (withinLat), _withinLon (withinLon), _crossesDateline (crossesDateline), _throwsException (throwsException) { }
};


class BoundsCrossingDatelineTest : public ::testing::TestWithParam<BoundsCrossingDatelineTestRow> {
protected:
    virtual void setUpTestCase() {

    }

    virtual void tearDownTestCase() {

    }
};
