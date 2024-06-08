#pragma once

template <typename V>
V fillVector(float start, float step) {
    auto vec = V();
    for(float nextVal = start; auto& col : vec) {
        col = nextVal;
        nextVal += step;
    }
    return vec;
}

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
