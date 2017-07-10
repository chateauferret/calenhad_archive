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
#include "IcosphereDivider.h"


using namespace icosphere;
using namespace Marble;


Icosphere::Icosphere (const int& depth) : Model (depth) {

}

void Icosphere::assemble (const Bounds& bounds) {
   _divider = new IcosphereDivider  (bounds, _depth);
    _divider -> divide (_depth);
    _vertices = _divider -> vertices();
    _lastVisited = _vertices [0];
}

Icosphere::~Icosphere() {
    if (_divider) {
        delete _divider;
    }
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
        if (lon < -M_PI_2) { lon += M_PI_2; }
        if (lon > M_PI_2) { lon -= M_PI_2; }
        for (int j = 0; j < image->height (); j++) {
            double lat = bounds.south () + (bounds.north () - bounds.south ()) / image->width () * j;
            if (lat < -M_PI) { lat += M_PI; }
            if (lat > M_PI) { lat -= M_PI; }
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
