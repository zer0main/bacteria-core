/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Model.hpp"
#include "constants.hpp"
#include "random.hpp"
#include "Exception.hpp"

namespace Abstract {

Model::Model(
    int /*width*/,
    int /*height*/
) {
}

CellState Model::cellState(int x, int y) const {
    return cellState_impl(x, y);
}

int Model::getDirection(int x, int y) const {
    return getDirection_impl(x, y);
}

int Model::getMass(int x, int y) const {
    return getMass_impl(x, y);
}

int Model::getTeam(int x, int y) const {
    return getTeam_impl(x, y);
}

int Model::getWidth() const {
    return getWidth_impl();
}

int Model::getHeight() const {
    return getHeight_impl();
}

}

namespace Implementation {

/* get global coordinate from horizontal and
   vertical coordinates
*/
static int getIndex(int x, int y, int width, int height) {
    bool less = ((x < 0) || (y < 0));
    bool greater = ((x >= width) || (y >= height));
    if (less || greater) {
        throw Exception("Model: index of cell in arguments "
                        "of some methods is out of range.");
    }
    int index = y * width + x;
    return index;
}

Unit::Unit(
    int mass,
    int direction,
    int team,
    int instruction
)
    : mass(mass)
    , direction(direction)
    , team(team)
    , instruction(instruction) {
}

Model::Model(
    int width,
    int height
)
    : Abstract::Model(width, height)
    , width_(width)
    , height_(height) {
    board_.resize(width * height, 0);
}

Abstract::CellState Model::cellState_impl(int x, int y) const {
    int index = getIndex(x, y, width_, height_);
    Unit* unit_ptr = board_[index];
    if (unit_ptr != 0) {
        return Abstract::Model::BACTERIUM;
    } else {
        return Abstract::Model::EMPTY;
    }
}

int Model::getDirection_impl(int x, int y) const {
    int index = getIndex(x, y, width_, height_);
    Unit* unit_ptr = board_[index];
    if (unit_ptr != 0) {
        return unit_ptr->direction;
    } else {
        throw Exception("Error: Attempt to get direction of empty cell.");
    }
}

int Model::getMass_impl(int x, int y) const {
    int index = getIndex(x, y, width_, height_);
    Unit* unit_ptr = board_[index];
    if (unit_ptr != 0) {
        return unit_ptr->mass;
    } else {
        throw Exception("Error: Attempt to get mass of empty cell.");
    }
}

int Model::getTeam_impl(int x, int y) const {
    int index = getIndex(x, y, width_, height_);
    Unit* unit_ptr = board_[index];
    if (unit_ptr != 0) {
        return unit_ptr->team;
    } else {
        // no unit in the current cell
        throw Exception("Error: Attempt to get team of empty cell.");
    }
}

int Model::getWidth_impl() const {
    return width_;
}

int Model::getHeight_impl() const {
    return height_;
}

void Model::initializeBoard(int bacteria, int teams) {
    for (int team = 0; team < teams; team++) {
        for (int bacterium = 0; bacterium < bacteria; bacterium++) {
            int direction = random(4);
            Unit unit(DEFAULT_MASS, direction, team, 0);
            units_[team].push_back(unit);
            int x = random(width_);
            int y = random(height_);
            board_[y * width_ + x] = &(units_[team][bacterium]);
        }
    }

}

}
