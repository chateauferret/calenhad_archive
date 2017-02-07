// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stack>

#include "icosphere.h"
#include "vertex.h"
#include "triangle.h"

using namespace geoutils;
using namespace icosphere;
using namespace noise::utils;


//--------------------------------------------------------------------------------
// icosahedron data
//--------------------------------------------------------------------------------

static constexpr double X = 0.525731112119133606;
static constexpr double Z = 0.850650808352039932;

static constexpr double vdata[12][3] = {
        {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
        {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
        {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

static constexpr unsigned tindices[20][3] = {
        {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
        {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
        {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
        {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

//--------------------------------------------------------------------------------


Icosphere::Icosphere (const char& depth, const Bounds& bounds) : Model (depth) {

    _bounds = bounds;

    std::cout << "\nBuilding icosphere to level " << (int) _levels << "\n";
    _gc = new GeographicLib::Geocentric (1.0, 0.0); // sphere
    _gd = new GeographicLib::Geodesic (1.0, 0.0);
    _rhumb = new GeographicLib::Rhumb (1.0, 0.0);


    // init with an icosahedron
     for (int i = 0; i < 12; i++) {
        Cartesian c = Cartesian (vdata [i][0], vdata[i][1], vdata [i][2]);
         Vertex* v = new Vertex (i, c, 0, _rhumb);
        _vertices.push_back (v);
    }
    _lastVisited = _vertices [0];
    _indices.push_back (new std::vector<unsigned>);
    std::vector<unsigned>& indices = *_indices.back();
    for (int i = 0; i < 20; i++) {
        for (int k = 0; k < 3; k++) {
            indices.push_back (tindices[i][k]);
        }
    }

    for (int i = 0; i < 20; i++) {
        int j = i * 3;
        makeNeighbours (indices [j], indices [j + 1]);
        makeNeighbours (indices [j], indices [j + 2]);
        makeNeighbours (indices [j + 1], indices [j + 2]);
        addTriangle (indices [j], indices [j + 1], indices [j + 2], 0, 0);
    }
    
    _listIds.push_back(0);
    while(_indices.size()<_levels) {
        subdivide (_indices.size());
    }
}

Icosphere::~Icosphere() {
    delete _gc;
    delete _gd;
    delete _rhumb;
    std::for_each (_vertices.begin(), _vertices.end(), [] (Vertex* p) { delete p; });
    std::for_each (_triangles.begin(), _triangles.end(), [] (std::pair<uint128_t, Triangle*> t) { delete t.second; });
}

const std::vector<unsigned>& Icosphere::indices (const unsigned int& level) {
    while (level >= _indices.size()) {
        subdivide (_indices.size());

    }
  return *_indices [level];
}

void Icosphere::subdivide (const unsigned int& level) {
    int count = 0;
    typedef unsigned long long Key;
    std::map<Key, unsigned> edgeMap;
    const std::vector<unsigned>& indices = *_indices.back();
    _indices.push_back (new std::vector<unsigned>);
    std::vector<unsigned>& refinedIndices = *_indices.back();
    unsigned end = indices.size();
    for (unsigned i = 0; i < end; i += 3) {
        // Subdivide each triangle if any part of it is in bounds, or if there are no bounds
        if (coversTriangle (_vertices [indices [i]] -> getGeolocation(), _vertices [indices [i + 1]] -> getGeolocation(), _vertices [indices [i + 2]] -> getGeolocation())) {
            unsigned ids0 [3];  // indices of outer vertices
            unsigned ids1 [3];  // indices of edge vertices
            for (int k = 0; k < 3; ++k) {
                int k1 = (k + 1) % 3;
                int e0 = indices [i + k];
                int e1 = indices [i + k1];

                ids0[k] = e0;
                if (e1 > e0) {
                    std::swap (e0, e1);
                }
                Key edgeKey = Key (e0) | (Key (e1) << 32);
                std::map<Key, unsigned>::iterator it = edgeMap.find (edgeKey);
                if (it == edgeMap.end ()) {
                    ids1 [k] = _vertices.size ();
                    edgeMap [edgeKey] = ids1 [k];
                    Vertex* mid = _vertices [e0];
                    Cartesian c = (mid->getCartesian()) + (_vertices [e1] -> getCartesian());
                    _vertex = new Vertex (ids1 [k], c, level, _rhumb);
                    _vertices.push_back (_vertex);
                } else {
                    ids1 [k] = it->second;
                }
                _vertex = _vertices [ids1 [k]];
            }
            Triangle* parent = _triangles.find (triangleKey (ids0 [0], ids0 [1], ids0 [2])) -> second;
            makeTriangle (refinedIndices, ids0 [0], ids1 [0], ids1 [2], level, parent);
            makeTriangle (refinedIndices, ids0 [1], ids1 [1], ids1 [0], level, parent);
            makeTriangle (refinedIndices, ids0 [2], ids1 [2], ids1 [1], level, parent);
            makeTriangle (refinedIndices, ids1 [0], ids1 [1], ids1 [2], level, parent);
            count++;
        }
    }
    _listIds.push_back (0);
}

void Icosphere::makeTriangle (std::vector<unsigned>& refinedIndices, const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* parent) {
    refinedIndices.push_back (a);
    refinedIndices.push_back (b);
    refinedIndices.push_back (c);
    makeNeighbours (a, b);
    makeNeighbours (b, c);
    makeNeighbours (a, c);

    addTriangle (a, b, c, level, parent);
}

void Icosphere::addTriangle (const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* parent) {

    // Don't add the triangle if none of it is in bounds
    //if (!_bounds || (_bounds -> contains (_vertices [a] -> getGeolocation()) || _bounds -> contains (_vertices [b] -> getGeolocation()) || _bounds -> contains (_vertices [c] -> getGeolocation()))) {

        uint128_t tkey = triangleKey (a, b, c);
        Triangle* t = new Triangle (_vertices [a], _vertices [b], _vertices [c], level);
        if (level > 0) {
            t -> setParent (parent);
            parent -> addChild (t);
        }
        _triangles.insert (std::make_pair (tkey, t));

        _vertices [a] -> addTriangle (t);
        _vertices [b] -> addTriangle (t);
        _vertices [c] -> addTriangle (t);
    //}
}

uint128_t Icosphere::triangleKey (unsigned a, unsigned b, unsigned c) {
    unsigned temp;
    if (a > b) { temp = a; a = b; b = temp; }
    if (a > c) { temp = a; a = c; c = temp; }
    if (b > c) { temp = b; b = c; c = temp; }
    uint128_t tkey = ((uint128_t) (a)) << 64 | ((uint128_t) (b)) << 32 | ((uint128_t) (c));

    return tkey;
}

void Icosphere::makeNeighbours (const unsigned& p, const unsigned& q) {
        _vertices [p] -> addNeighbour (_vertices [q]);
        _vertices [q] -> addNeighbour (_vertices [p]);
}

Vertex* Icosphere::operator [] (const unsigned& id) {
    return _vertices [id];
}

const std::vector<Vertex*> Icosphere::vertices() { return _vertices; }
unsigned Icosphere::vertexCount() { return _vertices.size(); }

Vertex* Icosphere::nearest (const Geolocation& target, const unsigned int& depth) const {

    double d, dist;
    Vertex* pV;
    _lastVisited = _vertices[0];
    for (int i = 1; i < 12; i++) {
        pV = _vertices[i];
        d = distance (pV->getGeolocation (), target);
        if (i == 1 || d < dist) {
            _lastVisited = pV;
            dist = d;
        }
    }

    // walk over the Delaunay triangulation until a point is reached that is nearer the key than any connected point
    return walkTowards (target, depth);

}

void Icosphere::visit (Vertex* pV) {
    _lastVisited = pV;
}

Vertex* Icosphere::walkTowards (const Geolocation& target, const unsigned int& depth) const {
    return walkTowards (Math::toCartesian (target), depth);
}

Vertex* Icosphere::walkTowards (const Cartesian& target, const unsigned int& depth) const {
  if (depth == 0 || depth > _levels - 1) { return walkTowards (target, _levels - 1); }
  double dist = Math::distSquared (_lastVisited -> getCartesian(), target);
  std::pair <std::set<Vertex*>::iterator, std::set<Vertex*>::iterator> n = _lastVisited -> getNeighbours();
  std::set<Vertex*>::iterator i  = n.first;
  Vertex* next = nullptr;

  bool found = false;
  while (i != n.second) {
      if ((*i) -> getLevel() <= depth) {
          double d = Math::distSquared ((*i) -> getCartesian(), target);
          if (d < dist) { // && (*i) -> getLevel() >= (_lastVisited -> getLevel())) {
              next = *i;
              dist = d;
              found = true;
            }
        }
      i++;
    }

  if (found) {
      _lastVisited = next;
      return walkTowards (target, depth);
    } else {

      return _lastVisited;
    }
}


std::experimental::optional<double> Icosphere::getDatum (const Geolocation& g, const std::string& key) {
    if (isInBounds (g, _bounds)) {
        std::map<std::string, Dataset>::iterator i = _datasets.find (key);
        if (i != _datasets.end()) {
            Dataset dataset = i -> second;
            Vertex* v = walkTowards (Math::toCartesian (g), dataset.getDepth());
            return v -> getDatum (key);
        } else {
            throw DatasetNotFoundException (key);
        }
    } else {
        throw IllegalIcosphereAccessException ("Search target out of bounds");
    }
}

bool Icosphere::setDatum (const Geolocation& g, const std::string& key, const double& datum) {
    std::map<std::string, Dataset>::iterator i = _datasets.find (key);
    if (i != _datasets.end()) {
        Dataset dataset = i -> second;
        Vertex* v = walkTowards (Math::toCartesian (g), dataset.getDepth());
        v -> setDatum (key, datum);
    } else {
        throw DatasetNotFoundException (key);
    }
}

// for now, images can't cross the dateline - this is OK in a tiling arrangement
bool Icosphere::getImage (QImage* image, const Bounds& bounds, const std::string& key) {
    std::map<std::string, Dataset>::iterator item = _datasets.find (key);
    std::cout << "Looking for dataset " << key << "\n";
    if (item != _datasets.end()) {
        Dataset dataset = item -> second;
        Legend* legend = dataset.getLegend();
        for (int i = 0; i < image -> width(); i++) {
            double lon = bounds.east + (bounds.west - bounds.east) / image -> height() * i;
            if (lon < -M_PI_2) { lon += M_PI_2; }
            if (lon > M_PI_2) { lon -= M_PI_2; }
            for (int j = 0; j < image -> height(); j++) {
                double lat = bounds.north + (bounds.south - bounds.north) / image -> width() * j;
                if (lat < -M_PI) { lat += M_PI; }
                if (lat > M_PI) { lat -= M_PI; }
                Vertex* v = walkTowards (Math::toCartesian (Geolocation (lat, lon, Units::Radians)));
                image -> setPixel (i, j, legend -> lookup (v -> getDatum (key)).rgb());
            }
        }
        return true;
    } else {
        throw DatasetNotFoundException (key);
    }
}

std::string Icosphere::getType() {
  return "Icosphere";
}

bool Icosphere::coversTriangle (const Geolocation& a, const Geolocation& b, const Geolocation& c, const Bounds& bounds) const {
    double minLat = std::min (std::min(a.longitude, b.longitude), c.longitude);
    double maxLat = std::max (std::max(a.longitude, b.longitude), c.longitude);
    if (maxLat - minLat > M_PI) {
        // then triangle is deemed to cross the dateline
        Geolocation a1 = Geolocation (a.longitude > M_PI_2 ? a.longitude : a.longitude - M_2_PI, a.latitude, Units::Radians);
        Geolocation b1 = Geolocation (b.longitude > M_PI_2 ? b.longitude : b.longitude - M_2_PI, b.latitude, Units::Radians);
        Geolocation c1 = Geolocation (c.longitude > M_PI_2 ? c.longitude : c.longitude - M_2_PI, c.latitude, Units::Radians);
        Geolocation a2 = Geolocation (a.longitude < -M_PI_2 ? a.longitude : a.longitude + M_2_PI, a.latitude, Units::Radians);
        Geolocation b2 = Geolocation (b.longitude < -M_PI_2 ? b.longitude : b.longitude + M_2_PI, b.latitude, Units::Radians);
        Geolocation c2 = Geolocation (c.longitude < -M_PI_2 ? c.longitude : c.longitude + M_2_PI, c.latitude, Units::Radians);
        return coversTriangle (a1, b1, c1, bounds) || coversTriangle (a2, b2, c2, bounds);
    }
    // triangle is in bounds if its vertices don't all lie beyond the same side of the box
    if (a.latitude < bounds.south && b.latitude < bounds.south && c.latitude < bounds.south) { return false; }
    if (a.latitude > bounds.north && b.latitude > bounds.north && c.latitude > bounds.north) { return false; }
    if (bounds.west > bounds.east) {
        // test on the longitude coordinates is inverted if the bounds cross the dateline
        if ((a.longitude < bounds.west && b.longitude < bounds.west && c.longitude < bounds.west &&
                a.longitude > 0 && b.longitude > 0 && c.longitude > 0)  ||
            (a.longitude > bounds.east && b.longitude > bounds.east && c.longitude > bounds.east &&
                a.longitude < 0 && b.longitude < 0 && c.longitude < 0)) { return false; }
    } else {
        if ((a.longitude < bounds.west && b.longitude < bounds.west && c.longitude < bounds.west) ||
        (a.longitude > bounds.east && b.longitude > bounds.east && c.longitude > bounds.east)) { return false; }
    }


    return true;
}


bool Icosphere::coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c) const {
    return coversTriangle (a, b, c, _bounds);
}


bool Icosphere::isInTriangle (const Geolocation& p1, const Geolocation& p2, const Geolocation& p3, double lon, double lat) const {
    // triangle is in bounds if any corner of the bounds is within it - compute barymetric coordinates - true if all these are positive
    double alpha = ((p2.latitude - p3.latitude)*(lon - p3.longitude) + (p3.longitude - p2.longitude)*(lat - p3.latitude)) /
                  ((p2.latitude - p3.latitude)*(p1.longitude - p3.longitude) + (p3.longitude - p2.longitude)*(p1.latitude - p3.latitude));
    double beta = ((p3.latitude - p1.latitude)*(lon - p3.longitude) + (p1.longitude - p3.longitude)*(lat - p3.latitude)) /
                  ((p2.latitude - p3.latitude)*(p1.longitude - p3.longitude) + (p3.longitude - p2.longitude)*(p1.latitude - p3.latitude));
    double gamma = 1.0d - alpha - beta;
    return alpha > 0 && beta > 0 && gamma > 0;
}

bool Icosphere::isInBounds (const Geolocation& a, const Bounds& bounds) const {
    return (a.latitude <= bounds.north && a.latitude >= bounds.south && a.longitude >= bounds.west && a.longitude <= bounds.east);
}
