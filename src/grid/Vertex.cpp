//
// Created by ferret on 05/02/2021.
//

#include "Vertex.h"

using namespace calenhad::grid;


Vertex::Vertex () {

}

Vertex::~Vertex () {
    delete _data;
}

Data::Data (Vertex* vertex) : _vertex (vertex) {

}

Data::~Data () {
    delete next;
}
