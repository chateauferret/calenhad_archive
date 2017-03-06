#include <limits.h>
#include <gtest/gtest.h>
#include "../icosphere/legend.h"
#include <iostream>
#include <random>
#include <QtGui/QtGui>
#include <libnoise/module/modulebase.h>
#include <libnoise/module/perlin.h>

#include "../libnoiseutils/intervallegend.h"
#include "../icosphere/dataset.h"
#include "../icosphere/icosphere.h"
#include "../icosphere/vertex.h"
#include "../libnoiseutils/IcosphereBuilder.h"
#include "../libnoiseutils/GradientLegend.h"


using namespace noise::utils;
using namespace icosphere;
using namespace geoutils;

class IcosphereTest : public ::testing::Test {
    protected:
    unsigned points;
    double maxdist, totaldist;
    long timeEnd, timeStart;
    IntervalLegend legend = IntervalLegend();
    QColor colours [8] = {
        QColor (0, 0, 0), QColor (0, 0, 255), QColor (0, 255, 0), QColor (0, 255, 255),
        QColor (255, 0, 0), QColor (255, 0, 255), QColor (255, 255, 0), QColor (255, 255, 255)
    };
    Dataset dataset = Dataset ("dataset", &legend, 5);
    Icosphere icosphere = Icosphere (5);
    time_t seed;
    std::default_random_engine e;
    std::uniform_real_distribution<double> random;


    virtual void SetUp() override {
        for (int i = 0; i < 8; i++) {
            legend.addEntry (100 * i, colours [i]);
        }
        maxdist = 0.0;
        totaldist = 0.0;
        seed = time (NULL);
        e = std::default_random_engine (seed);
        random = std::uniform_real_distribution<double> (0.0, 1.0);
        timeStart = clock() / static_cast<double> (CLOCKS_PER_SEC);

    }

    virtual void TearDown() override {

    }
};

TEST_F (IcosphereTest, buildSpeed) {
    for (int i = 2; i < 9; i++) {
        double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
        Icosphere icosphere = Icosphere (i);
        double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
        std::cout << "Built icosphere level " << i << " with " << icosphere.vertexCount() << " vertices in " << end - start << " seconds" << "\n";
    }
}

TEST_F (IcosphereTest, imageSpeed) {
    Icosphere icosphere = Icosphere (8);
    IntervalLegend legend = IntervalLegend();
    icosphere.addDataset ("dataset", &legend, 8);

    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
    QImage* image = new QImage (256, 256, QImage::Format_RGB888);
    Geolocation nw (30, 30, Units::Degrees);
    Geolocation se (60, 0, Units::Degrees);
    int i;
    for (i = 0; i < 10; i++) {
        EXPECT_TRUE (icosphere.getImage (image, icosphere::Bounds (nw.latitude, se.latitude, se.longitude, se.latitude), "dataset"));
    }
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built " << i << " images in " << end - start << " seconds" << "\n";
    delete image;
}

TEST_F (IcosphereTest, searchTimes) {
    Icosphere icosphere = Icosphere (8);
    time_t seed = time (NULL);
    std::default_random_engine e = std::default_random_engine (seed);
    std::uniform_real_distribution<double> random = std::uniform_real_distribution<double> (0.0, 1.0);
    double timeStart = clock() / static_cast<double> (CLOCKS_PER_SEC);
    double maxdist = 0.0, totaldist = 0.0;
    unsigned points;
    for (points = 0; points < 100; points++) {
        double lat = (random (e) - 0.5) * M_PI;
        double lon = (random (e) - 0.5) * M_PI * 2;
        Geolocation sought = Geolocation (lat, lon,Units::Radians);
        Vertex* found = icosphere.nearest (sought);
        double dist = icosphere.distance (found -> getGeolocation(), sought);
        maxdist = dist > maxdist ? dist : maxdist;
        totaldist += dist;
    }
   double timeEnd = clock() / static_cast<double> (CLOCKS_PER_SEC);

    EXPECT_LT (totaldist / points, maxdist * 2);
    EXPECT_LT (totaldist / points, 0.02);

    std::cout << "Found " << points <<  " points in " << timeEnd - timeStart << " seconds (" << (timeEnd - timeStart) / points * 1000 << " ms per point)\n";
    std::cout << "Maximum distance from a vertex: " << maxdist << " radians" << "\n";
    std::cout << "Average distance from a point: " << (totaldist / points) << " radians" << "\n";
}


// Exercising class IcosphereBounds and its use

class IcosphereBoundsTestRow {
public:
    double _n, _s, _w, _e;
    int _count;
    IcosphereBoundsTestRow (const double& n, const double& s, const double& w, const double& e, const int& count) : _n (n), _s (s), _w (w), _e (e), _count (count) { }
};

class IcosphereBoundsTest : public ::testing::TestWithParam<IcosphereBoundsTestRow> {
protected:
    virtual void SetUp() override { }
    virtual void TearDown() override { }
};


TEST_P (IcosphereBoundsTest, bounds) {

    IcosphereBoundsTestRow const& row = GetParam();
    icosphere::Bounds bounds (degreesToRadians (row._n), degreesToRadians (row._s), degreesToRadians (row._w), degreesToRadians (row._e));
    std::cout << row._n << "N, " << row._w << "W - " << row._s << "S " << row._e << "E\n";
    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
    Icosphere icosphere = Icosphere (8, bounds);
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Estimated " << bounds . estimateVertexCount (8) << " vertices\n";
    std::cout << "Built icosphere with " << icosphere.getCountInBounds() << " vertices in bounds; merged << " << icosphere.getCountMerged () << ": " << end - start << " seconds" << "\n";
    EXPECT_EQ (icosphere.vertexCount(), row._count);

}

INSTANTIATE_TEST_CASE_P (bounds, IcosphereBoundsTest, ::testing::Values (
        IcosphereBoundsTestRow (90, -90, -180, 180, 163842),
        IcosphereBoundsTestRow (30, 0, 0, 30, 4374),
        IcosphereBoundsTestRow (60, 0, 0, 60, 13514),
        IcosphereBoundsTestRow (30, -30, -30, 30, 15108),
        IcosphereBoundsTestRow (30, 0, -150, 150, 38944), // not across dateline
        IcosphereBoundsTestRow (30, 0, 150, -150, 6855), // across dateline
        IcosphereBoundsTestRow (30, 0, 90, -90, 21873)); // across dateline
);

// Exercising class IcosphereBuilder


class IcosphereBuilderTestRow {
public:
    double _n, _s, _w, _e;          // icosphere bounds
    IcosphereBuilderTestRow (const double& n, const double& s, const double& w, const double& e) : _n (n), _s (s), _w (w), _e (e) { }
};

class IcosphereBuilderTest : public ::testing::TestWithParam<IcosphereBuilderTestRow> {
protected:

    GradientLegend* legend;
    noise::module::Perlin* perlin;

    virtual void SetUp() override {
        legend = new noise::utils::GradientLegend();
        perlin = new noise::module::Perlin();
    }

    virtual void TearDown() override {
        delete perlin;
        delete legend;
    }


};

TEST_P (IcosphereBuilderTest, builder) {
    IcosphereBuilderTestRow const& row = GetParam ();
    IcosphereBuilder builder = IcosphereBuilder (8, Bounds (degreesToRadians (row._n), degreesToRadians (row._s), degreesToRadians (row._w), degreesToRadians (row._e)));

    builder.build ("altitude", legend, perlin);
    Icosphere* icosphere = builder.icosphere ();
    EXPECT_TRUE (icosphere -> hasDataset ("altitude"));
    for (int i = 1000; i < icosphere->vertexCount (); i += 100) {
        Vertex* v = (*icosphere) [i];
        Cartesian c = v -> getCartesian ();
        Vertex* found = icosphere -> nearest (Math::toGeolocation (c));
        double foundValue = found -> getDatum ("altitude").value ();
        EXPECT_EQ (v -> getId(), found -> getId());
        EXPECT_NEAR (Math::toGeolocation (c).latDegrees, found -> getGeolocation().latDegrees, 0.00001);
        EXPECT_NEAR (Math::toGeolocation (c).lonDegrees, found -> getGeolocation().lonDegrees, 0.00001);
        EXPECT_EQ (foundValue, perlin -> GetValue (c.x, c.y, c.z));
    }
    std::cout << "Region " << row._n << "N, " << row._w << "W - " << row._s << "S, " << row._e << "E with " << icosphere -> vertexCount() << " vertices \n";
}

INSTANTIATE_TEST_CASE_P (builder, IcosphereBuilderTest, ::testing::Values (
        IcosphereBuilderTestRow (90, -90, -180, 180),
        IcosphereBuilderTestRow (30, -30, -30, 30),
        IcosphereBuilderTestRow (30, -30, 150, -150));
);

// Exercising icosphere bounds checking for searches

class IcosphereBoundsCheckTestRow {
public:
    double _lat, _lon;
    bool _ok;
    IcosphereBoundsCheckTestRow (const double& lat, const double& lon, const bool& ok) : _lat (lat), _lon (lon), _ok (ok) { }
};

class IcosphereBoundsCheckTest : public ::testing::TestWithParam<IcosphereBoundsCheckTestRow> {
protected:

    GradientLegend* legend;
    noise::module::Perlin* perlin;

    virtual void SetUp() override {
        legend = new noise::utils::GradientLegend();
        perlin = new noise::module::Perlin();
    }

    virtual void TearDown() override {
        delete perlin;
        delete legend;
    }
};

TEST_P (IcosphereBoundsCheckTest, boundsCheck) {
    IcosphereBoundsCheckTestRow const& row = GetParam ();
    IcosphereBuilder builder = IcosphereBuilder (8, Bounds (degreesToRadians (30), degreesToRadians (0), degreesToRadians (0), degreesToRadians (30)));

    builder.build ("altitude", legend, perlin);
    Icosphere* icosphere = builder.icosphere ();

    // when we ask the icosphere for a value out of bounds it throws an exception

    if (!row._ok) {
        EXPECT_THROW ({
                          double value = icosphere -> getDatum (Geolocation (row._lon, row._lat, Units::Degrees), "altitude").value ();
                      }, IllegalIcosphereAccessException);
    } else {
        EXPECT_NO_THROW ({
                             double value = icosphere -> getDatum (Geolocation (row._lon, row._lat, Units::Degrees), "altitude").value ();
                         });
    }
}
INSTANTIATE_TEST_CASE_P (boundsCheck, IcosphereBoundsCheckTest, ::testing::Values (
        IcosphereBoundsCheckTestRow (15, 15, true),
        IcosphereBoundsCheckTestRow (15, 60, false),
        IcosphereBoundsCheckTestRow (15, -15, false),
        IcosphereBoundsCheckTestRow (45, 15, false),
        IcosphereBoundsCheckTestRow (15, 45, false));
);
