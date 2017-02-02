#include "triangle.h"

using namespace geoutils;
using namespace icosphere;

Triangle::Triangle (const Vertex *v1, const Vertex *v2, const Vertex *v3, const unsigned& l) : a (v1), b (v2), c (v3), level (l) {
}

Triangle::Triangle (const Triangle& other) : Triangle (other.a, other.b, other.c, other.level) {
}

bool Triangle::operator= (const Triangle& other) {
    return (a == other.a) && (b == other.b) && (c == other.c);
}

void Triangle::setParent (Triangle* t) {
    parent = t;
}

void Triangle::addChild (Triangle* t) {
    children.insert (t);
}

Triangle* Triangle::getParent() {
    return parent;
}

std::set<Triangle*> Triangle::getChildren (const unsigned& depth) {
    std::set<Triangle*> result;
    result.insert (children.begin(), children.end());
    
    if (depth > 0) {
        std::set<Triangle*>::iterator i;
        for (i = children.begin(); i != children.end(); i++) {
            Triangle* t = *i;
            std::set<Triangle*> add = t -> getChildren (depth - 1);
            result.insert (add.begin(), add.end());
        }
    }
    return result;
}

Geolocation Triangle::getCentre() {
    return Math::toGeolocation (centre);
}

