//
// Created by ferret on 18/10/2020.
//

#include "Tectonics.h"

using namespace calenhad::grid;
using namespace geoutils;

Tectonics::Tectonics (const unsigned int& depth) : Icosphere (depth), _time (0) {

    // Random number distribution
    _rand = std::uniform_real_distribution<double> (-1.0, 1.0);  //(min, max)

}

Tectonics::~Tectonics () {
    std::map<int, Plate*>::iterator i = _plates.begin();
    while (i != _plates.end()) {
        delete i -> second;
        i++;
    }
}

void Tectonics::initialise (const int& seed, const int& plateCount) {
    qDebug ("Tectonics::initialise");
    // Initialise the random number generator with the seed
    _seq = std::mt19937 (seed);
    createPlates (plateCount);
}

void Tectonics::run () {
    qDebug ("Tectonics::run");
    bool complete = false;
    while (! complete) {
        _time++;
        cycle();
        if (_time == 10000) { complete = true; }
    }
}

void Tectonics::createPlates (const int& count) {
    qDebug ("Tectonics::createPlates");
    for (int i = 0; i < count; i++) {
        Plate* p = new Plate();
        p -> _pole = Geolocation (_rand (_seq) * M_PI, _rand (_seq) * M_PI * 2);
        p -> _centre = Geolocation (_rand (_seq) * M_PI, _rand (_seq) * M_PI * 2);
        geometry::LatLon pos (p -> _centre.latitude(), p -> _centre.longitude());
        toCartesian (pos, p -> _c);
        p -> _angVelocity = _rand (_seq) * MAX_ANGULAR_VELOCITY;
        p -> _id = _plates.size();
        _plates.insert (std::make_pair (p -> _id, p));
    }

}

void Tectonics::cycle() {

}

void Tectonics::createInitialCrust (CubicSphere* oceanic, CubicSphere* continental) {
    qDebug ("Tectonics::createInitialCrust");
    for (Vertex* v : _vertices) {
        geometry::Cartesian c1 = v -> cartesian;

        Plate* p = findPlate (c1);
        Mass* m = new Mass();
        m -> _plate = p;
        m -> _timeCreated = p -> _angVelocity < 0 ? 0 : -1;        // -1 for continental: age not tracked
        m -> _vertex = v;
        geometry::LatLon pos;
        toLatLon (c, pos);
        m -> _where = pos;
        geoutils::Cartesian c (c1._x, c1._y, c1._z);
        if (m -> _timeCreated < 0) {
            // continental crust
            m -> _thickness = continental -> valueAt (c);
        } else {
            // oceanic crust
            m -> _thickness = oceanic -> valueAt (c);
        }
        _lithosphere.push_back (m);
        if (v -> id % 10000 == 0) {
            std::cout << ".";
        }
    }
    std::cout << "\n";
}

Plate* Tectonics::findPlate (const geometry::Cartesian& c) {
    Plate* p = _plates.begin() -> second;
    double min = 0.0;
    std::map<int, Plate*>::iterator i = _plates.begin();
    while (i != _plates.end()) {
        if (i == _plates.begin () || distSquared (c, i -> second -> _c) < min) {
            p = i -> second;
        }
        i++;
    }
    return p;
}

void Tectonics::createNewCrust (Vertex* v) {
    Mass* m = new Mass();
    m -> _vertex = v;
    m -> _timeCreated = _time;
    geometry::LatLon pos;
    toLatLon (v -> cartesian, pos);
    m -> _where = pos;
    m -> _thickness = NEW_CRUST_THICKNESS;
    m -> _plate = findPlate (v -> cartesian);
    _lithosphere.push_back (m);
}
