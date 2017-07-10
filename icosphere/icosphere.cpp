// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stack>
#include <libnoise/module/modulebase.h>
#include "icosphere.h"
#include "marble/GeoDataLineString.h"


using namespace icosphere;
using namespace Marble;

constexpr double Icosphere::vdata[12][3];
constexpr unsigned Icosphere::tindices[20][3];

Icosphere::Icosphere (const int& depth) : Model (depth) {
    _gc = new GeographicLib::Geocentric (1.0, 0.0); // sphere
    _gd = new GeographicLib::Geodesic (1.0, 0.0);
    _rhumb = new GeographicLib::Rhumb (1.0, 0.0);
}

void Icosphere::assemble (const Bounds& bounds) {
    _bounds = bounds;
    std::cout << "Building icosphere to level " << (int) _depth << "\n";
    purge();
    emit progress (0);
    _toDo = (double) _bounds.estimateVertexCount (_depth);
    // init with an icosahedron
     for (int i = 0; i < 12; i++) {
        Cartesian c = Cartesian (vdata [i][0], vdata[i][1], vdata [i][2]);
        _vertices.push_back (new Vertex (i, c, 0, _rhumb));
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
    while(_indices.size()<_depth) {
        subdivide (_indices.size());
    }
}

Icosphere::~Icosphere() {
    delete _gc;
    delete _gd;
    delete _rhumb;
   purge();
}

void Icosphere::purge() {
    std::for_each (_vertices.begin(), _vertices.end(), [] (Vertex* p) { delete p; });
    _vertices.clear();
    std::for_each (_triangles.begin(), _triangles.end(), [] (std::pair<boost::uint128_type, Triangle*> t) { delete t.second; });
    _triangles.clear();
    std::for_each (_indices.begin(), _indices.end(), [] (std::vector<unsigned>* v) { v -> clear(); delete v; });
    _indices.clear();
    _listIds.clear();

}

const std::vector<unsigned>& Icosphere::indices (const unsigned int& level) {
    while (level >= _indices.size()) {
        const_cast<Icosphere*>(this) -> subdivide (_indices.size());
    }
  return *_indices [level];
}

void Icosphere::subdivide (const unsigned int& level) {
    int count = 0, prog = 0;
    typedef unsigned long long Key;
    std::map<Key, unsigned> edgeMap;
    const std::vector<unsigned>& indices = *_indices.back();
    _indices.push_back (new std::vector<unsigned>);
    std::vector<unsigned>& refinedIndices = *_indices.back();
    unsigned end = indices.size();
    int temp;

    for (unsigned i = 0; i < end; i += 3) {
        if (coversTriangle (_vertices [indices [i]] -> getGeolocation(), _vertices [indices [i + 1]] -> getGeolocation(), _vertices [indices [i + 2]] -> getGeolocation())) {
            unsigned ids0[3];  // indices of outer vertices
            unsigned ids1[3];  // indices of edge vertices
            for (int k = 0; k < 3; ++k) {
                int k1 = (k + 1) % 3;
                int e0 = indices[i + k];
                int e1 = indices[i + k1];
                ids0[k] = e0;
                if (e1 > e0) {
                    temp = e0;
                    e0 = e1;
                    e1 = temp;
                }
                Key edgeKey = Key (e0) | (Key (e1) << 32);
                std::map<Key, unsigned>::iterator it = edgeMap.find (edgeKey);
                if (it == edgeMap.end ()) {
                    ids1[k] = _vertices.size ();
                    edgeMap[edgeKey] = ids1[k];
                    Vertex* mid = _vertices[e0];
                    Cartesian c = (mid->getCartesian ()) + (_vertices[e1]->getCartesian ());
                    _vertex = new Vertex (ids1[k], c, level, _rhumb);
                    _vertices.push_back (_vertex);
                } else {
                    ids1[k] = it->second;
                }
                _vertex = _vertices[ids1[k]];
            }
            Triangle* parent = _triangles.find (_triangleKey (ids0[0], ids0[1], ids0[2]))->second;
            makeTriangle (refinedIndices, ids0[0], ids1[0], ids1[2], level, parent);
            makeTriangle (refinedIndices, ids0[1], ids1[1], ids1[0], level, parent);
            makeTriangle (refinedIndices, ids0[2], ids1[2], ids1[1], level, parent);
            makeTriangle (refinedIndices, ids1[0], ids1[1], ids1[2], level, parent);
            count++;
        }
        int p  = (int) (((double) vertexCount() / _toDo) * 100);
        if (p != prog) {
            emit progress (p);
            prog = p;
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
    boost::uint128_type tkey = _triangleKey (a, b, c);
    Triangle* t = new Triangle (_vertices [a], _vertices [b], _vertices [c], level);
    if (level > 0) {
        t -> setParent (parent);
        parent -> addChild (t);
    }
    _triangles.insert (std::make_pair (tkey, t));

    _vertices [a] -> addTriangle (t);
    _vertices [b] -> addTriangle (t);
    _vertices [c] -> addTriangle (t);

}

boost::uint128_type Icosphere::_triangleKey (unsigned a, unsigned b, unsigned c) {
    unsigned temp;
    if (a > b) { temp = a; a = b; b = temp; }
    if (a > c) { temp = a; a = c; c = temp; }
    if (b > c) { temp = b; b = c; c = temp; }
    boost::uint128_type tkey = boost::uint128_type (a) << 64 | boost::uint128_type (b) << 32 | boost::uint128_type (c);
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

Vertex* Icosphere::nearest (const Geolocation& target, const unsigned int& depth) const{
    double d, dist;
    Vertex* pV;
    _lastVisited = _vertices [0];
    for (int i = 1; i < 12; i++) {
        pV = _vertices [i];
        d = distance (pV -> getGeolocation(), target);
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
  if (depth == 0 || depth > _depth - 1) { return walkTowards (target, _depth - 1); }
  double dist = Math::distSquared (_lastVisited -> getCartesian(), target);
  std::pair <std::set<Vertex*>::iterator, std::set<Vertex*>::iterator> n = _lastVisited -> getNeighbours();
  std::set<Vertex*>::iterator i  = n.first;
  Vertex* next;

  bool found = false;
  while (i != n.second) {
      if ((*i) -> getLevel() <= depth) {
          double d = Math::distSquared ((*i) -> getCartesian(), target);
          if (d < dist && (*i) -> getLevel() >= (_lastVisited -> getLevel())) {
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


bool Icosphere::coversTriangle (const Geolocation& a, const Geolocation& b, const Geolocation& c, const Bounds& bounds) const {
    double minLat = std::min (std::min(a.longitude, b.longitude), c.longitude);
    double maxLat = std::max (std::max(a.longitude, b.longitude), c.longitude);
    if (maxLat - minLat > M_PI) {
        // then triangle is deemed to cross the dateline
        Geolocation a2 = Geolocation (a.longitude < -M_PI_2 ? a.longitude : a.longitude + M_2_PI, a.latitude, Units::Radians);
        Geolocation b2 = Geolocation (b.longitude < -M_PI_2 ? b.longitude : b.longitude + M_2_PI, b.latitude, Units::Radians);
        Geolocation c2 = Geolocation (c.longitude < -M_PI_2 ? c.longitude : c.longitude + M_2_PI, c.latitude, Units::Radians);
        return coversTriangle (a2, b2, c2, bounds);
    }
    /*/ triangle is in bounds itf its vertices don't all lie beyond the same side of the _box
    if (a.latitude < bounds.south() && b.latitude < bounds.south() && c.latitude < bounds.south()) { return false; }
    if (a.latitude > bounds.north() && b.latitude > bounds.north() && c.latitude > bounds.north()) { return false; }
    if (bounds.crossesDateline()) {
        // test on the longitude coordinates is inverted if the bounds cross the dateline
        if ((a.longitude < bounds.west() && b.longitude < bounds.west() && c.longitude < bounds.west() &&
             a.longitude > 0 && b.longitude > 0 && c.longitude > 0)  ||
            (a.longitude > bounds.east() && b.longitude > bounds.east() && c.longitude > bounds.east() &&
             a.longitude < 0 && b.longitude < 0 && c.longitude < 0)) { return false; }
    } else {
        if ((a.longitude < bounds.west() && b.longitude < bounds.west() && c.longitude < bounds.west()) ||
            (a.longitude > bounds.east() && b.longitude > bounds.east() && c.longitude > bounds.east())) { return false; }
    } */

    GeoDataLatLonBox box (bounds.north(), bounds.south(), bounds.east(), bounds.west());
    GeoDataLineString ls;
    ls.append (GeoDataCoordinates (a.longitude, a.latitude));
    ls.append (GeoDataCoordinates (b.longitude, b.latitude));
    ls.append (GeoDataCoordinates (c.longitude, c.latitude));
    GeoDataLatLonBox triangleBox = GeoDataLatLonBox::fromLineString (ls);
    return (box.intersects (triangleBox));
}


bool Icosphere::coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c) const {
    return coversTriangle (a, b, c, _bounds);
}

double Icosphere::getDatum (const Geolocation& g, const QString& key) {
    QMutexLocker locker (&_lockMutex);
    if (! _locked) {
        Vertex* v = walkTowards (Math::toCartesian (g), _depth);
        return v->getDatum (key).value_or (0.0);
    } else return 0.0;
}

void Icosphere::setDatum (const Geolocation& g, const QString& key, double datum) {
    Vertex* v = walkTowards (Math::toCartesian (g), _depth);
    v -> setDatum (key, datum);
}


std::string Icosphere::getType() {
  return "Icosphere";
}

const unsigned Icosphere::depth () {
    return _depth;
}

const Bounds& Icosphere::bounds () {
    return _bounds;
}


// for now, images can't cross the dateline - this is OK in a tiling arrangement
bool Icosphere::getImage (QImage* image, Legend* legend, const Bounds& bounds) {
    for (int i = 0; i < image -> width(); i++) {
        double lon = bounds.east () + (bounds.east () - bounds.west ()) / image->height () * i;
        if (lon < -HALF_PI) { lon += HALF_PI; }
        if (lon > HALF_PI) { lon -= HALF_PI; }
        for (int j = 0; j < image->height (); j++) {
            double lat = bounds.south () + (bounds.north () - bounds.south ()) / image->width () * j;
            if (lat < -PI) { lat += PI; }
            if (lat > PI) { lat -= PI; }
            Vertex* v = walkTowards (Math::toCartesian (Geolocation (lat, lon, Radians)));
            image->setPixel (i, j, legend->lookup (v->getDatum ("")).rgb ());
        }
    }
    return true;
}

void Icosphere::lock () {
    QMutexLocker locker (&_lockMutex);
    _locked = true;
}


void Icosphere::unlock () {
    QMutexLocker locker (&_lockMutex);
    _locked = false;
}
