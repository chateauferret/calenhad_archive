//
// Created by ferret on 18/10/2020.
//

#include "Tectonics.h"

using namespace calenhad::grid;
using namespace geoutils;

Tectonics::Tectonics (const unsigned int& depth)  : Icosphere (depth), _time (0) {

    // Random number distribution
    _rand = std::uniform_real_distribution<double> (-1.0, 1.0);  //(min, max)

}

Tectonics::~Tectonics () {
    for (Plate* i : _plates) {
        delete i;
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
    //cycle();
}

void Tectonics::createPlates (const int& count) {
    qDebug ("Tectonics::createPlates");
    for (int i = 0; i < count; i++) {
        Plate* p = new Plate();
        p -> _pole = Geolocation (_rand (_seq) * M_PI, _rand (_seq) * M_PI * 2);
        p -> _centre = Geolocation (_rand (_seq) * M_PI, _rand (_seq) * M_PI * 2);
        p -> _angVelocity = _rand (_seq) * MAX_ANGULAR_VELOCITY;
        p -> _id = i;
        std::cout << "Plate " << p -> _id << ": pole " << p -> _pole.latitude (Degrees) << ", " << p -> _pole.longitude (Degrees) << ", ang vel " << p -> _angVelocity << "\n";
        _plates.push_back (p);
    }

}

void Tectonics::createInitialCrust() {

    // create mass for centre of each plate and assign it to the plate
    for (Plate* p : _plates) {
        Vertex* v = nearest (p -> _centre);
        createMass (v, p);
        _todo.push_back (v);
    }

    int count = 0;
    while (! _todo.empty()) {
        ulong i = (ulong) _rand (_seq) * _todo.size();
        Vertex* v = _todo.at (i);
        count++;
        for (Vertex* n : v -> neighbours) {
            Mass* data = (Mass*) v -> _data;
            if (! n -> _data) {
                createMass (n, data -> _plate);
                _todo.push_back (n);
            }

        }
        _todo.remove (i);
    }
}

void Tectonics::cycle() {
    _time++;
    // Work out where each _vertex goes under it's plate's Euler rotation and store that position in its data.
    for (Vertex* v : _vertices) { move (v); }
    for (Vertex* v : _vertices) { makeFeatures ((Mass*) v -> _data); }

}

void Tectonics::move (Vertex* v) const {
    if (v -> _updatedAt < _time) {
        v -> _updatedAt = _time;
        Mass* m = (Mass*) v -> _data;
        Plate* p = m -> _plate;

        // compute new position of mass
        Cartesian vec = geoutils::Geoutils::toCartesian (m -> _where);
        Cartesian c = Geoutils::rotate (vec, p -> _pole, (float) p -> _angVelocity);
        m -> _whereTo = Geoutils::toGeolocation (c);

    }
}

void Tectonics::makeFeatures (Mass* vm) {
    Vertex* v = vm -> _vertex;
    Vertex* w = walkTowards (vm -> _whereTo);
    Mass* wm = (Mass*) w -> _data;
    Plate* vp = vm -> _plate;
    Plate* wp = wm -> _plate;

    if (wm && (vp != wp)) {
            if (v -> _isContinental && w -> _isContinental) {
                // uralian or himalayan orogeny
               // wm -> _thickness += vm -> _thickness * HIMALAYAN_OROGENY_FACTOR;
                delete vm; v -> _data = nullptr;
            }
            if (v -> _isContinental && ! w -> _isContinental) {
                // andean or laramide orogeny
              //  wm -> _thickness += vm -> _thickness * ANDEAN_OROGENY_FACTOR;
                delete vm; v -> _data = nullptr;
            }
            if (! v -> _isContinental && ! w -> _isContinental) {
                if (vm -> _timeCreated < wm -> _timeCreated) {
                    // island arc formation
              //      wm -> _thickness += vm -> _thickness * ISLAND_ARC_FACTOR;
                    delete vm; v -> _data = nullptr;
                }
            }


        }

}

void Tectonics::updateLithosphere() {

    // Compute interactions between plates at each boundary _vertex
    for (Vertex* v : _vertices) {

    }
}

void Tectonics::createMass (Vertex* v, Plate* p) const {
    Mass* m = new Mass();
    m -> _plate = p;
    m -> _timeCreated = _time;
    m -> _vertex = v;
    v -> _data = m;
    m -> _where = Geoutils::toGeolocation (v -> cartesian);

    if (p -> _id < 10) {// continental crust
        m -> _thickness = INITIAL_CONT_CRUST_THICKNESS;
    } else {
        // oceanic crust
        m -> _thickness = NEW_CRUST_THICKNESS;
    }
    v -> value = m -> _thickness;

}

Plate* Tectonics::findPlate (const Cartesian& c) {
    double min;
    Plate* found = _plates.first();
    for (Plate* p : _plates) {
        double d = distSquared (c, p -> _c);
        if (p == _plates.first() || d < min) {
            min = d;
            found = p;
        }
    }
    return found;
}

void Tectonics::createNewCrust (Vertex* v) {
    Mass* m = new Mass();
    m -> _vertex = v;
    m -> _timeCreated = _time;
    m -> _where = Geoutils::toGeolocation (v -> cartesian);
    m -> _thickness = NEW_CRUST_THICKNESS;
}
