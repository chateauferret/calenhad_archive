#include <limits.h>
#include <gtest/gtest.h>
#include "../libnoiseutils/legend.h"
#include <iostream>
#include <random>
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
    Geolocation nw (30, 30, Geolocation::DEGS);
    Geolocation se (60, 0, Geolocation::DEGS);
    int i;
    for (i = 0; i < 10; i++) {
        EXPECT_TRUE (icosphere.getImage (image, GeoQuad (nw, se), "dataset"));
    }
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built " << i << " images in " << end - start << " seconds" << "\n";
    delete image;
}

/*
// OSGEarth version
TEST (IcosphereTest, imageSpeed) {
    Icosphere icosphere = Icosphere (8);
    IntervalLegend legend = IntervalLegend();
    const std::string key = "dataset";
    icosphere.addDataset (key, &legend, 8);
    const unsigned size = 256;
    unsigned char* data = new unsigned char [size * size * 3];
    double start = clock() / static_cast<double> (CLOCKS_PER_SEC);

    Geolocation nw (30, 30, Geolocation::DEGS);
    Geolocation se (60, 0, Geolocation::DEGS);
    GeoQuad bounds = GeoQuad (nw, se);
    int i;
    for (i = 0; i < 10; i++) {
        EXPECT_TRUE (icosphere.getImage (data, bounds, key, size));
    }
    double end = clock() / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built " << i << " images in " << end - start << " seconds" << "\n";
    delete data;
}
*/
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
        Geolocation sought = Geolocation (lat, lon, Geolocation::RADS);
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

TEST (IcosphereTest, searchLevels) {
  Icosphere icosphere = Icosphere (8);
  Geolocation target = Geolocation (45, 45, Geolocation::DEGS);
  for (int i = 2; i != 8; i++) {
      Vertex* v = icosphere.nearest (target, i);
      EXPECT_LT (v -> getLevel(), i + 1);
    }
}

TEST (IcosphereTest, vertices) {
    Icosphere icosphere = Icosphere (8);

    GeoQuad bounds = GeoQuad (Geolocation (30, 30, Geolocation::DEGS), Geolocation (60, 60, Geolocation::DEGS), Geolocation (45, 45, Geolocation::DEGS));
    for (int i = 1; i <= 7; i++) {
        int expected = 0;
        double timeStart = clock() / static_cast<double> (CLOCKS_PER_SEC);
        int count = 0, outwith = 0;
        std::set<Vertex*>* vertices = icosphere.getVertices (bounds, i);
        for (Vertex* v : *vertices) {
            if (bounds.contains (v -> getGeolocation())) {
              count++;
            } else {
                outwith++;
            }
         }


        for (Vertex* v : icosphere.vertices()) {
          if (bounds.contains (v -> getGeolocation()) && v -> getLevel() <= i) {
              expected++;
            }
        }

        double timeEnd = clock() / static_cast<double> (CLOCKS_PER_SEC);
        std::cout << "Depth " << i << ": found " << count << " in bounds, " << outwith << " outside in " << timeEnd - timeStart << " seconds ";
        std::cout << "Should have " << expected << " vertices\n";
        delete vertices;
     }

}
/*
TEST (IcosphereTest, mesh) {
    Icosphere icosphere = Icosphere (8);
    GeoQuad bounds = GeoQuad (Geolocation (30, 30, Geolocation::DEGS), Geolocation (35, 35, Geolocation::DEGS));
    std::vector<double> vertices;
    std::vector<unsigned> indices;
    icosphere.mesh (bounds, 7, vertices, indices);
}


TEST (IcosphereTest, modelService) {
  ModelService::open();
  Model* model = ModelService::getModel();
   std::cout <<"Model of type " << model -> getType() << "\n";
   QImage* _buffer = new QImage (256, 256, QImage::Format_RGB32);
   GeoQuad _bounds =  GeoQuad (Geolocation (-20, -20, Geolocation::DEGS), Geolocation (20, 20, Geolocation::DEGS));
   std::string _key = "elevation";
   bool ok = model -> getImage (_buffer, _bounds, _key);
}
*/