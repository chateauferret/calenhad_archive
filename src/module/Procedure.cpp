//
// Created by ferret on 06/09/2020.
//

#include "Procedure.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"
#include "../graph/ComputeService.h"


using namespace calenhad::nodeedit;
using namespace calenhad::grid;
using namespace calenhad::graph;

calenhad::module::Procedure::Procedure (const QString& nodeType) : Cache (nodeType), _buffersIn { nullptr, nullptr, nullptr, nullptr} {

}

calenhad::module::Procedure::~Procedure () {

}

void calenhad::module::Procedure::refresh () {
    ComputeService* c = CalenhadServices::compute();
    if (c && isComplete()) {
        delete _buffer;
        _buffer = new CubicSphere (CalenhadServices::preferences() -> calenhad_compute_gridsize);

        // fill the input buffers from their sources
        for (int i = 0; i < std::max (_inputs.size (), 4); i++) {
            Port* p = inputs ().value (i);
            Module* source = p -> connections ().first () -> otherEnd (p) -> owner();
            c -> compute (source, _buffersIn [i]);
        }

        // compute the output buffer
        c -> process (this, _buffer);

    }
}

CubicSphere* calenhad::module::Procedure::inputBuffer (const int& i) {
    return _buffersIn [i];
}

int calenhad::module::Procedure::iterations() {
    return (int) parameterValue ("iterations");
}
