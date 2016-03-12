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

CellState Model::cellState(int x, int y) const {
    return cellState_impl(x, y);
}

int Model::getTeam(int x, int y) const {
    return getTeam_impl(x, y);
}

}

namespace Implementation {

static void tryToPlace(int team) {
    while (true) {
        int x = random(size);
        int y = random(size);
        if (cellState(x, y) != Abstract::Model::BACTERIUM) {
            board_[y * size + x] = team;
            break;
        }
    }
}

Model::Model(int size, int bacteria_number, int teams_number) :
    Abstract::Model(size, bacteria_number, teams_number) {
    board_.resize(size * size, Abstract::Model::EMPTY);
    for (int team = 0; team < teams_number; team++) {
        for (int bacteria = 0; bacteria < bacteria_number; bacteria++) {
            tryToPlace(team);
        }
    }
}

Abstract::Model::CellState Model::cellState_impl(int x, int y) const {
    int size = board_.size();
    return (board_[y * size + x] != 0);
}

int Model::getTeam_impl(int x, int y) const {
    int size = board_.size();
    int res = board_[y * size + x];
    if (res != 0) {
        return res;
    } else {
        throw Exception("Error: Attempt to get team of empty cell.");
    }
}

}
