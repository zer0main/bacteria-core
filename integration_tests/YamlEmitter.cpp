/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "YamlEmitter.hpp"

void YamlEmitter::writeWidth(int width) {
    out_["board_width"] = width;
}

void YamlEmitter::writeHeight(int height) {
    out_["board_height"] = height;
}

void YamlEmitter::writeUnits(const Units& units) {
    for (int i = 0; i < units.size(); i++) {
        out_["units"][i] = units[i];
    }
}
