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
    int /*width*/,
    int /*height*/
) {
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

Model::Model(
    int width,
    int height
)
    : Abstract::Model(width, height)
    , width_(width)
    , height_(height) {
    board_.resize(width * height);
}

int Model::getTeam_impl(int x, int y) const {
    int res = board_[y * width_ + x].team;
    return res;
}

int Model::getWidth_impl() const {
    return width_;
}

int Model::getHeight_impl() const {
    return height_;
}

}
