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

Cache::Cache (const QString &nodeType) : Convolution (nodeType) {

}

Cache::~Cache() {

}

void Cache::invalidate() {
    refresh();
    Convolution::invalidate();

}

void Cache::refresh() {
    ComputeService* c = CalenhadServices::compute();
    int size = CalenhadServices::gridSize();
    delete _buffer;
    //_buffer = (GLfloat*) malloc (_size * _size * 2 * sizeof (GLfloat));
    _buffer = new CubicSphere();
    Port* p = inputs().first();
    Module* source = p -> connections().first() -> otherEnd (p) -> owner();
    c -> compute (source, _buffer);
}
