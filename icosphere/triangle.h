#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "vertex.h"
#include "../geoutils.h"
#include <set>
#include "icosphereutils.h"

namespace icosphere {

    class Vertex;

    class Triangle {
    public:
        const Vertex* a, * b, * c;
        geoutils::Cartesian centre;
        unsigned int level;
        Triangle* parent;
        std::set<Triangle*> children;

        Triangle (const Vertex* v1, const Vertex* v2, const Vertex* v3, const unsigned& l);

        Triangle (const Triangle& other);

        bool operator= (const Triangle& other);

        void setParent (Triangle* t);

        void addChild (Triangle* t);

        Triangle* getParent ();

        std::set<Triangle*> getChildren (const unsigned& depth);

        geoutils::Geolocation getCentre ();
    };

}
#endif // TRIANGLE.H
