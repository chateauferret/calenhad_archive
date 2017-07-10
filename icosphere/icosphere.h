#ifndef ICOSPHERE_OLD_H
#define ICOSPHERE_OLD_H
#include <iostream>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Rhumb.hpp>
#include <vector>
#include <map>
#include "vertex.h"
#include "../geoutils.h"
#include <QBuffer>
#include <QImage>
#include "../mapping/Legend.h"
#include "triangle.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <QtCore/QMutex>
#include "Model.h"
#include "Bounds.h"

using namespace geoutils;

namespace noise {
    namespace module {
        class Module;
    }
}

namespace icosphere {

    class IcosphereDivider;
    
    class Icosphere : public Model {
    Q_OBJECT
    public:

       

        Icosphere (const int& depth);
        ~Icosphere();
        const std::vector<Vertex*> vertices();
        const std::vector<unsigned>& indices (const unsigned int& level);
        unsigned vertexCount();
        Vertex* operator [] (const unsigned& id);
        Vertex* nearest (const Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const Cartesian& target, const unsigned int& depth = 0) const;
        void visit (Vertex* vertex);
        bool getImage (QImage* image, Legend* legend, const Bounds& bounds);

        void setDatum (const Geolocation& g, const QString& key, double datum) override;
        double getDatum (const Geolocation& g, const QString& key) override;
        std::string getType();
        const unsigned depth();
        const Bounds& bounds();
        void lock();
        void unlock();
        void assemble (const Bounds& bounds = Bounds());

    signals:
        void progress (const int&);

    protected:
        std::vector<Vertex*> _vertices;
        mutable Vertex* _vertex;
        mutable Vertex* _lastVisited;
        IcosphereDivider* _divider;
        Bounds _bounds = Bounds();
        bool _locked;
        QMutex _lockMutex;
    };
} // namespace
#endif // ICOSPHERE_OLD_H
