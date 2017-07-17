#ifndef ICOSPHERE_OLD_H
#define ICOSPHERE_OLD_H
#include <iostream>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Rhumb.hpp>
#include <vector>
#include <map>
#include <memory>
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

    typedef std::vector<Vertex*> VertexList;

    class Icosphere : public Model {
    Q_OBJECT
    public:



        Icosphere (const int& depth);
        ~Icosphere();
        const std::shared_ptr<VertexList> vertices();
        unsigned vertexCount();
        Vertex* operator [] (const unsigned& id);
        Vertex* nearest (const Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const Cartesian& target, const unsigned int& depth = 0) const;
        void visit (Vertex* vertex);
        bool getImage (QImage* image, Legend* legend, const Bounds& bounds, const QString& key = "");

        void setDatum (const Geolocation& g, const QString& key, double datum) override;
        double getDatum (const Geolocation& g, const QString& key) override;
        std::string getType();
        const unsigned depth();
        const Bounds& bounds();
        void lock();
        void unlock();
        void assembleAsync (const Bounds& bounds = Bounds ());
        void assemble (const Bounds& bounds);

    signals:
        void progress (const int&);
        void ready();

    protected:
        std::shared_ptr<VertexList> _vertices;
        mutable Vertex* _vertex;
        mutable Vertex* _lastVisited;
        IcosphereDivider* _divider;
        Bounds _bounds = Bounds();
        bool _locked;
        QMutex _lockMutex;

        private slots:
        void assembled();


    };
} // namespace
#endif // ICOSPHERE_OLD_H
