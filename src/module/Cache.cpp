//
// Created by ferret on 07/08/2020.
//

#include "Cache.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"

using namespace calenhad::module;
using namespace calenhad::graph;
using namespace calenhad::grid;
using namespace calenhad::nodeedit;

Cache::Cache (const QString &nodeType) : StructuredGrid (nodeType) {

}

Cache::~Cache() {

}

void Cache::invalidate() {
    refresh();
    StructuredGrid::invalidate();

}

void Cache::refresh() {
    ComputeService* c = CalenhadServices::compute();
    if (c && isComplete()) {
        delete _buffer;
        _buffer = new CubicSphere();
        Port* p = inputs().first();
        Module* source = p -> connections().first() -> otherEnd(p) -> owner();

        // copy data from the source module into the structured grid buffer
        c -> compute (source, _buffer);


    }
}
