/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Model.hpp"
#include "random.hpp"
#include "Exception.hpp"

namespace Abstract {

Model::Model(
    int /*size*/,
    int /*bacteria_number*/,
    int /*teams_number*/
) {
}

Model::CellState Model::cellState(int x, int y) const {
    return cellState_impl(x, y);
}

int Model::getTeam(int x, int y) const {
    return getTeam_impl(x, y);
}

int Model::size() const {
    return size_impl();
}

}

namespace Implementation {

Model::Model(
    int size,
    int bacteria_number,
    int teams_number
)
    : Abstract::Model(size, bacteria_number, teams_number)
    , size_(size) {
    board_.resize(size * size, Abstract::Model::EMPTY);
    for (int team = 1; team <= teams_number; team++) {
        for (int bacteria = 0; bacteria < bacteria_number; bacteria++) {
            tryToPlace(team);
        }
    }
}

Abstract::Model::CellState Model::cellState_impl(int x, int y) const {
    if (board_[y * size_ + x] != 0) {
        return Abstract::Model::BACTERIUM;
    } else {
        return Abstract::Model::EMPTY;
    }
}

int Model::getTeam_impl(int x, int y) const {
    int res = board_[y * size_ + x];
    if (res != 0) {
        return res;
    } else {
        throw Exception("Error: Attempt to get team of empty cell.");
    }
}

int Model::size_impl() const {
    return size_;
}

void Model::tryToPlace(int team) {
    while (true) {
        int x = random(size_);
        int y = random(size_);
        if (cellState(x, y) != Abstract::Model::BACTERIUM) {
            board_[y * size_ + x] = team;
            break;
        }
    }
}

}
