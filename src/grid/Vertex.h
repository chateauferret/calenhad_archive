//
// Created by ferret on 05/02/2021.
//

#ifndef CALENHAD_VERTEX_H
#define CALENHAD_VERTEX_H

#include <geoutils.h>
#include <list>

namespace calenhad {
    namespace grid {


        struct Vertex;

        class Data {

        public:
            Vertex* _vertex;

            explicit Data (Vertex* vertex = nullptr);
            ~Data();
            Data* next = nullptr;

        };

        struct Vertex {
        public:
            Vertex();
            ~Vertex();
            geoutils::Cartesian cartesian;
            std::list<Vertex*> edges;              // vertices connected to this one at any level by an edge
            std::list<Vertex*> neighbours;          // immediate neighbours of this _vertex
            uint32_t id;
            double value = -0.5;
            unsigned level;
            Data* _data = nullptr;
            bool _isContinental;
            int _updatedAt = 0;
        };
    }
}


#endif //CALENHAD_VERTEX_H
