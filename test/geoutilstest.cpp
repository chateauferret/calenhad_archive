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
    Geolocation g1 (row.latRads, row.lonRads, Geolocation::RADS);
    Cartesian c1 = Math::toCartesian (g1);
    EXPECT_NEAR (row.x, c1.x, TOLERANCE);
    EXPECT_NEAR (row.y, c1.y, TOLERANCE);
    EXPECT_NEAR (row.z, c1.z, TOLERANCE);

    Geolocation g2 (row.latDegrees, row.lonDegrees, Geolocation::DEGS);
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


TEST_P (BoundsCrossingDatelineTest, bounds) {
    
    BoundsCrossingDatelineTestRow const& row = GetParam ();
    try {
        GeoQuad bounds = GeoQuad (Geolocation (row._nwLat, row._nwLon, Geolocation::DEGS), Geolocation (row._seLat, row._seLon, Geolocation::DEGS), Geolocation (row._withinLat, row._withinLon, Geolocation::DEGS));
        if (row._throwsException) {
            FAIL ();
        }
        // test crossesDateline
        EXPECT_EQ (row._crossesDateline, bounds.crossesDateline ());

        // test rationalise
        std::pair <std::vector<GeoQuad>::iterator, std::vector<GeoQuad>::iterator> r = bounds.rationalise();
        int count = 0;
        for (std::vector<GeoQuad>::iterator i = r.first; i < r.second; i++) {
            EXPECT_EQ (false, i -> crossesDateline ());
            count++;
        }
        EXPECT_EQ (count, bounds.crossesDateline () ? 2 : 1);
    } catch (const IllegalGeoCoordinatesException& e) {
        if (row._throwsException) {
            ASSERT_STREQ ("Point given must lie within latitude range of bounds", e.what());
        } else {
            FAIL();
        }
    }
}

INSTANTIATE_TEST_CASE_P (bounds, BoundsCrossingDatelineTest, ::testing::Values (
        BoundsCrossingDatelineTestRow (20, 20, 40, 40, 30, 30, false, false),
        BoundsCrossingDatelineTestRow (20, 20, 40, 40, 30, 60, true, false),
        BoundsCrossingDatelineTestRow (20, 20, 40, 40, 60, 30, false, true),
        BoundsCrossingDatelineTestRow (20, 20, 40, 40, 60, 30, true, true));
);

class BoundsCentreTestRow {
public:
    double _nwLat, _nwLon, _seLat, _seLon, _withinLat, _withinLon, _centreLat, _centreLon;    
    BoundsCentreTestRow (const double& nwLat, const double& nwLon, const double& seLat, const double& seLon, const double& withinLat, const double& withinLon, const double& centreLat, const double& centreLon) :
        _nwLat (nwLat), _nwLon (nwLon), _seLat (seLat), _seLon (seLon), _withinLat (withinLat), _withinLon (withinLon), _centreLat (centreLat), _centreLon (centreLon) { }
};


class BoundsCentreTest : public ::testing::TestWithParam<BoundsCentreTestRow> {
protected:
    virtual void setUpTestCase() {

    }

    virtual void tearDownTestCase() {

    }
};


TEST_P (BoundsCentreTest, bounds) {
    
    BoundsCentreTestRow const& row = GetParam ();
    GeoQuad bounds = GeoQuad (Geolocation (row._nwLat, row._nwLon, Geolocation::DEGS), Geolocation (row._seLat, row._seLon, Geolocation::DEGS), Geolocation (row._withinLat, row._withinLon, Geolocation::DEGS));
    EXPECT_NEAR (row._centreLat, bounds.centre ().latDegrees, TOLERANCE);
    EXPECT_NEAR (row._centreLon, bounds.centre().lonDegrees, TOLERANCE);
}

INSTANTIATE_TEST_CASE_P (bounds, BoundsCentreTest, ::testing::Values (
        BoundsCentreTestRow (20, 20, 40, 40, 30, 30, 30, 30),
        BoundsCentreTestRow (0, 160, 20, -140, 10, -160, 10, -170),
        BoundsCentreTestRow (30, 30, 60, 60, 40, 40, 45, 45));
);

class BoundsContainsTestRow {
public:
  double _nwLat, _nwLon, _seLat, _seLon, _withinLat, _withinLon, _testLat, _testLon;
  bool _result;
  BoundsContainsTestRow (const double& nwLat, const double& nwLon, const double& seLat, const double& seLon, const double& withinLat, const double& withinLon, const double& testLat, const double& testLon, const bool& result) :
    _nwLat (nwLat), _nwLon (nwLon), _seLat (seLat), _seLon (seLon), _withinLat (withinLat), _withinLon (withinLon), _testLat (testLat), _testLon (testLon), _result (result) { }
};


class BoundsContainsTest : public ::testing::TestWithParam<BoundsContainsTestRow> {
protected:
    virtual void setUpTestCase() {

    }

    virtual void tearDownTestCase() {

    }
};

TEST_P (BoundsContainsTest, bounds) {

    BoundsContainsTestRow const& row = GetParam ();
    GeoQuad bounds = GeoQuad (Geolocation (row._nwLat, row._nwLon, Geolocation::DEGS), Geolocation (row._seLat, row._seLon, Geolocation::DEGS), Geolocation (row._withinLat, row._withinLon, Geolocation::DEGS));
    Geolocation geolocation = Geolocation (row._testLat, row._testLon, Geolocation::DEGS);
    EXPECT_EQ (row._result, bounds.contains (geolocation));
}

INSTANTIATE_TEST_CASE_P (bounds, BoundsContainsTest, ::testing::Values (
        BoundsContainsTestRow (20, 20, 40, 40, 30, 30, 30, 30, true),
        BoundsContainsTestRow (20, 20, 40, 40, 30, 30, 70, 70, false),
        BoundsContainsTestRow (0, 160, 20, -140, 10, -160, 10, -170, true),
        BoundsContainsTestRow (0, 160, 20, -140, 10, -160, 0, 0, false),
        BoundsContainsTestRow (30, 30, 60, 60, 40, 40, 45, 45, true));
);
