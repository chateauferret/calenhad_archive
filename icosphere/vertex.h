/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vertex.h
 * Author: martin
 *
 * Created on 30 September 2015, 17:22
 */

#ifndef VERTEX_H
#define VERTEX_H

#include <array>
#include "../geoutils.h"
#include <set>
#include <map>
#include <GeographicLib/Rhumb.hpp>
#include "icosphereutils.h"

namespace icosphere {
    class Triangle;

    class TriangleComparator {
    public:
        TriangleComparator ();

        TriangleComparator (const TriangleComparator& other);

        TriangleComparator (GeographicLib::Rhumb* rhumb, geoutils::Geolocation& pole);

        geoutils::Geolocation pole;

        bool operator() (Triangle* t1, Triangle* t2);

    protected:
        GeographicLib::Rhumb* _rhumb;
        geoutils::Geolocation _pole;
    };

    class Vertex {
    public:
        typedef std::set<Triangle*, TriangleComparator> TriangleSet;

        Vertex ();

        Vertex (unsigned i, geoutils::Cartesian& c, const unsigned& l, GeographicLib::Rhumb* r);

        Vertex (const Vertex& other);

        virtual ~Vertex ();

        geoutils::Geolocation getGeolocation () const;

        geoutils::Cartesian getCartesian () const;

        unsigned getLevel () const;

        void addNeighbour (Vertex* pV);

        std::pair<std::set<Vertex*>::iterator, std::set<Vertex*>::iterator> getNeighbours () const;

        int countNeighbours (const unsigned int& depth = 0);

        int getDatum (const std::string& dataset);

        void setDatum (const std::string& dataset, const int& value);

        void erase (const std::string& key);

        unsigned getId () const;

        void addTriangle (Triangle* t);

        std::vector<geoutils::Geolocation>::iterator getCell (const unsigned& level);


    protected:
        unsigned _id;
        unsigned _level;
        GeographicLib::Rhumb* _rhumb;
        geoutils::Cartesian _cartesian;
        geoutils::Geolocation _geolocation;
        double _a1, _a2, _length;
        std::map<std::string, int> _data;           // map dataset names to the datum of that dataset for this vertex
        std::set<Vertex*> _neighbours;              // neighbouring vertices
        std::map<unsigned, TriangleSet> _triangles;
    };
}
#endif /* VERTEX_H */

