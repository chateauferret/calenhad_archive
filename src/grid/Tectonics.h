//
// Created by ferret on 18/10/2020.
//

#ifndef CALENHAD_TECTONICS_H
#define CALENHAD_TECTONICS_H

#include <random>
#include <queue>
#include "icosphere.h"
#include <s2/s2point_index.h>
#include <s2/s2point.h>

// constants
#define MAX_ANGULAR_VELOCITY 0.02
#define NEW_CRUST_THICKNESS 0.1
#define INITIAL_CONT_CRUST_THICKNESS 0.5


namespace calenhad {
    namespace grid {

        class Plate {
        public:
            Plate () { };
            geoutils::Geolocation _pole;
            geoutils::Geolocation _centre;
            geoutils::Cartesian _c;
            double _angVelocity;
            int _id;

        };

        class Data;

        class Mass : public Data {
        public:
            Mass() : Data(), _where (0.0, 0.0) { };
            geoutils::Geolocation _where;
            double _thickness;
            double _hardness;
            double _timeCreated;
            Plate* _plate;
            Vertex* prevVertex;
            geoutils::Geolocation _whereTo;
        };

        class Tectonics : public Icosphere {
        public:
            explicit Tectonics (const unsigned int& depth = 8);
            ~Tectonics();
            void initialise (const int& seed, const int& plateCount);
            void createInitialCrust();

            void run();


        private:

            int _time;
            void createPlates (const int& count);
            void cycle();
            std::uniform_real_distribution<double> _rand;
            std::mt19937 _seq;

            void createNewCrust (Vertex* v);

            QVector<Plate*> _plates;
            QVector<Vertex*> _todo;

            Plate* findPlate (const geoutils::Cartesian& c);

            void createMass (Vertex* v, Plate* p) const;

            void updateLithosphere();

            void move (Vertex* v) const;
            void makeFeatures (Mass* m);
        };
    }
}


#endif //CALENHAD_TECTONICS_H
