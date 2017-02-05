#include <limits.h>
#include <gtest/gtest.h>
#include "../libnoiseutils/legend.h"
#include <iostream>
#include <random>
#include <QtGui/QtGui>
#include "../libnoiseutils/intervallegend.h"
#include "../icosphere/dataset.h"
#include "../icosphere/icosphere.h"
#include "../icosphere/vertex.h"
#include "../geoutils.h"
#include "../icosphere/model.h"


using namespace noise::utils;
using namespace icosphere;
using namespace geoutils;

class IcosphereBoundsTestRow {
public:
    double _n, _s, _w, _e;
    IcosphereBoundsTestRow (const double& n, const double& s, const double& w, const double& e) : _n (n), _s (s), _w (w), _e (e) { }
};

class IcosphereTest {
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


    virtual void SetUpTestCase() {
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

    virtual void TearDownTestCase() {

    }
};

TEST (IcosphereTest, buildSpeed) {
    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
    Icosphere i = Icosphere (8);
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built icosphere with " << i.vertexCount() << " vertices in " << end - start << " seconds" << "\n";
}

TEST (IcosphereTest, imageSpeed) {
    Icosphere icosphere = Icosphere (8);
    IntervalLegend legend = IntervalLegend();
    icosphere.addDataset ("dataset", &legend, 8);

    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
    QImage* image = new QImage(256, 256, QImage::Format_RGB888);
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

TEST (IcosphereTest, searchTimes) {
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


class IcosphereBoundsTest : public ::testing::TestWithParam<IcosphereBoundsTestRow> {
protected:
    virtual void setUpTestCase() {

    }

    virtual void tearDownTestCase() {

    }
};


TEST_P (IcosphereBoundsTest, bounds) {

    IcosphereBoundsTestRow const& row = GetParam();
    icosphere::Bounds bounds (degreesToRadians (row._n), degreesToRadians (row._s), degreesToRadians (row._w), degreesToRadians (row._e));
    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
    Icosphere icosphere = Icosphere (8, bounds);
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built icosphere with " << icosphere.vertexCount() << " vertices in " << end - start << " seconds" << "\n";

}

INSTANTIATE_TEST_CASE_P (bounds, IcosphereBoundsTest, ::testing::Values (
        IcosphereBoundsTestRow (90, -90, -180, 180),
        IcosphereBoundsTestRow (30, 0, 0, 30),
        IcosphereBoundsTestRow (60, 0, 0, 60),
        IcosphereBoundsTestRow (30, -30, -30, 30),
        IcosphereBoundsTestRow (30, 0, -150, 150), // not across dateline
        IcosphereBoundsTestRow (30, 0, 150, -150), // across dateline
        IcosphereBoundsTestRow (30, 0, -180, 180)); // across dateline
);