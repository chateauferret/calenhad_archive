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

class IcosphereTestRow {
public:
    IcosphereTestRow () { }

protected:


};


class IcosphereTest : public ::testing::TestWithParam<IcosphereTestRow> {
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
    Icosphere i = Icosphere (2);
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

TEST (IcosphereTest, bounds) {
    icosphere::Bounds bounds (degreesToRadians (60), degreesToRadians (-60), degreesToRadians (-60), degreesToRadians (60));
    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);
    Icosphere icosphere = Icosphere (8, bounds);
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built icosphere with " << icosphere.vertexCount() << " vertices in " << end - start << " seconds" << "\n";
    Geolocation target = Geolocation (45, 45, Units::Degrees);
    for (int i = 2; i != 8; i++) {
        Vertex* v = icosphere.nearest (target, i);
        EXPECT_LT (v -> getLevel(), i + 1);
    }
}