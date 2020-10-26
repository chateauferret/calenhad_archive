//
// Created by ferret on 18/10/2020.
//

#ifndef CALENHAD_TECTONICS_H
#define CALENHAD_TECTONICS_H

#include <random>
#include "icosphere.h"

// constants
#define MAX_ANGULAR_VELOCITY 0.02;
#define NEW_CRUST_THICKNESS 0.1;


namespace calenhad {
    namespace grid {

        class Plate {
        public:
            Plate () { };
            geoutils::Geolocation _pole;
            geoutils::Geolocation _centre;
            geometry::Cartesian _c;
            double _angVelocity;
            bool _isContinental;
            int _id;
        };

        class Mass {
        public:
            Mass() : _where (0.0, 0.0) { };
            geometry::LatLon _where;
            double _thickness;
            double _timeCreated;
            Plate* _plate;
            Vertex* _vertex;
        };

        class Tectonics : public Icosphere {
        public:
            explicit Tectonics (const unsigned int& depth = 8);
            ~Tectonics();
            void initialise (const int& seed, const int& plateCount);
            void createInitialCrust (CubicSphere* oceanic, CubicSphere* continental);

            void run();


        private:

            int _time;
            void createPlates (const int& count);
            void cycle();
            std::uniform_real_distribution<double> _rand;
            std::mt19937 _seq;
            std::vector<Mass*> _lithosphere;

            void createNewCrust (Vertex* v);

            std::map<int, Plate*> _plates;

            Plate* findPlate (const geometry::Cartesian& c);
        };
    }
}


#endif //CALENHAD_TECTONICS_H
