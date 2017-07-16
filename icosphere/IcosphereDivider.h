//
// Created by martin on 10/07/17.
//

#ifndef CALENHAD_ICOSPHEREDIVIDER_H
#define CALENHAD_ICOSPHEREDIVIDER_H

#include <boost/config.hpp>
#include <GeographicLib/Rhumb.hpp>
#include <geoutils.h>
#include "Bounds.h"
#include "icosphere.h"

namespace icosphere {
    class Vertex;
    class Triangle;

    class IcosphereDivider : public QObject {
        Q_OBJECT
    public:
        //--------------------------------------------------------------------------------
        // icosahedron data
        //--------------------------------------------------------------------------------
        static constexpr double X = 0.525731112119133606;
        static constexpr double Z = 0.850650808352039932;
        static constexpr double PI = 3.141592653589793;
        static constexpr double TWO_PI = 6.283185307179586;
        static constexpr double HALF_PI = 1.570796326794896;
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

        IcosphereDivider (const Bounds& bounds, const int& depth, std::shared_ptr<VertexList> vertices );
        virtual ~IcosphereDivider();
        std::shared_ptr<VertexList> vertices();

        public slots:
        void divide();

        signals:
        void progress (const int&);
        void complete();

    protected:
        Bounds _bounds;
        int _depth;

        std::shared_ptr<VertexList> _vertices;
        std::vector<std::vector<unsigned>*> _indices;
        std::vector<unsigned> _listIds;
        std::map<boost::uint128_type, Triangle*> _triangles;
        void purge ();
        void makeTriangle (std::vector<unsigned>& refinedIndices, const unsigned& a, const unsigned& b, const unsigned& c, const unsigned int& level, Triangle* parent);
        void addTriangle (const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* nparent);
        boost::uint128_type _triangleKey (unsigned a, unsigned b, unsigned c);
        void makeNeighbours (const unsigned& p, const unsigned& q);
        GeographicLib::Rhumb* _rhumb;
        double _toDo;
        void subdivide (const unsigned int &level);
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c) const;

        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c, const Bounds& bounds) const;

        Vertex* _vertex;


    };
}

#endif //CALENHAD_ICOSPHEREDIVIDER_H
