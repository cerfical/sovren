#pragma once

template <typename M>
M fillMatrix(float start, float step) {
    auto mat = M();
    for(float nextVal = start; auto& row : mat) {
        for(auto& col : row) {
            col = nextVal;
            nextVal += step;
        }
    }
    return mat;
}
