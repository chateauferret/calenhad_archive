//
// Created by ferret on 07/02/2021.
//

#include "FractalGrid.h"

calenhad::grid::FractalGrid::FractalGrid (const int& depth, const int& sampleDepth, const int& seed, CubicSphere* buffer) : Icosphere (depth),
    _sampleDepth (sampleDepth), _seed (seed), _buffer (buffer) {
}

void calenhad::grid::FractalGrid::setValue (calenhad::grid::Vertex* v) {
    if (v -> level <= _sampleDepth) {
        v -> value = _buffer -> valueAt (v -> cartesian);
    } else {
        v -> value = 0.0;
        for (Vertex* w : v -> edges) {
            if (w -> level == v -> level -1) {
                v -> value += w -> value;
            }
        }
        v -> value /= 6;
        // to do - perturb the value
    }
}
