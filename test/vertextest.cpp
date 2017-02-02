#include <limits.h>
#include <gtest/gtest.h>
#include <GeographicLib/Geocentric.hpp>
#include "../geoutils.h"
#define TOLERANCE 0.00000001

using namespace geoutils;

class VertexTestRow {
public:
    double latDegrees, lonDegrees, latRads, lonRads, x, y, z;
    VertexTestRow (const double& latd, const double&  lond, const double& latr, const double& lonr, const double& newx, const double& newy, const double& newz) :
        latDegrees (latd), lonDegrees (lond), latRads (latr), lonRads (lonr), x (newx), y (newy), z  (newz) { }
};


class VertexTest : public ::testing::TestWithParam<VertexTestRow> {
protected:
    virtual void setUpTestCase() {

    }

    virtual void tearDownTestCase() {

    }
};


TEST_P (VertexTest, conversion) {

    VertexTestRow const& row = GetParam();
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

INSTANTIATE_TEST_CASE_P (conversion, VertexTest, ::testing::Values (
    VertexTestRow (45, 45, M_PI / 4, M_PI / 4, 1.0/2.0, 1.0/2.0, 1 / sqrt (2) ));
);
