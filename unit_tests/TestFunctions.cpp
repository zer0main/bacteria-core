/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "TestFunctions.hpp"

Implementation::Model* createBaseModel(
    int bacteria,
    int teams
) {
    Implementation::Model* model =
        Abstract::makeModel<Implementation::Model>(
            MIN_WIDTH,
            MIN_HEIGHT,
            bacteria,
            teams
        );
    return model;
}
