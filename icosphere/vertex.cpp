/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vertex.cpp
 * Author: martin
 * 
 * Created on 30 September 2015, 17:22
 */

#include "vertex.h"
#include "triangle.h"

using namespace geoutils;
using namespace icosphere;

TriangleComparator::TriangleComparator () {}
TriangleComparator::TriangleComparator (const TriangleComparator& other) : _rhumb (other._rhumb), _pole (other._pole) { }
TriangleComparator::TriangleComparator (GeographicLib::Rhumb* rhumb, Geolocation& pole) : _rhumb (rhumb), _pole (pole) { }
bool TriangleComparator:: operator() (Triangle* t1, Triangle* t2) {
    double a1, a2, d;
    geoutils::Geolocation c1 = geoutils::Math::toGeolocation (t1 -> centre);
    Geolocation c2 = Math::toGeolocation (t2 -> centre);
    _rhumb -> Inverse (pole.latitude, pole.longitude, c1.latitude, c1.longitude, d, a1);
    _rhumb -> Inverse (pole.latitude, pole.longitude, c2.latitude, c2.longitude, d, a2);
    return a1 < a2;
}

Vertex::Vertex() {
}

Vertex::Vertex (unsigned i, Cartesian& c, const unsigned int& l, GeographicLib::Rhumb* r) : _id (i), _level (l), _rhumb (r), _cartesian (c), _geolocation (Math::toGeolocation (_cartesian)) {
}

Vertex::Vertex (const Vertex& other) : _id (other._id), _level (other._level), _rhumb (other._rhumb), _cartesian (other._cartesian), _geolocation (other._geolocation) {
}

unsigned Vertex::getId() const {
    return _id;
}

Vertex::~Vertex() {
}

void Vertex::addNeighbour (Vertex* vertex) {
    _neighbours.insert (vertex);
    
}

int Vertex::countNeighbours (const unsigned int& depth) {
    if (depth == 0) {
        return _neighbours.size();
    } else {
        return count_if (_neighbours.begin(), _neighbours.end(), [depth] (Vertex* v) {return (v -> _level <= depth);} );
    }
}

void Vertex::erase (const std::string& key) {
    _data.erase (key);
}

std::pair <std::set<Vertex*>::iterator, std::set<Vertex*>::iterator> Vertex::getNeighbours() const {
  return std::make_pair (_neighbours.begin(), _neighbours.end());
}

void Vertex::setDatum (const std::string& dataset, const int& value) {
    _data [dataset] = value;
}

int Vertex::getDatum (const std::string& dataset) {
    std::map<std::string, int>::iterator i = _data.find (dataset);
    if (i == _data.end()) {
        return 0; // to do - optional no value
    } else {
        return i -> second;
    }
}

void Vertex::addTriangle (Triangle* t) {
    TriangleSet ts;
    std::map<unsigned, TriangleSet>::iterator tsi = _triangles.find (t -> level);
    if (tsi == _triangles.end()) {
        ts = TriangleSet (TriangleComparator (_rhumb, _geolocation));
        _triangles.insert (make_pair (t -> level, ts));
    } else {
        ts = tsi -> second;
    }
    ts.insert (t);
}

std::vector<geoutils::Geolocation>::iterator Vertex::getCell (const unsigned& level) {
    std::vector<Geolocation> points;
    std::map<unsigned, TriangleSet>::iterator tsi = _triangles.find (level);
    if (tsi == _triangles.end()) {
        return points.end ();
    } else {
        TriangleSet ts = tsi -> second;
        for (TriangleSet::iterator i = ts.begin(); i != ts.end(); ++i) {
            Triangle* t = *i;
            points.push_back (t -> getCentre());
        }
        Triangle* t = * (ts.begin());
        points.push_back (t -> getCentre());  // repeat first point at the end to close the polygon
        return points.begin();
    }
}

Geolocation Vertex::getGeolocation() const { return _geolocation; }
Cartesian Vertex::getCartesian() const { return _cartesian; }
unsigned Vertex::getLevel() const { return _level; }


