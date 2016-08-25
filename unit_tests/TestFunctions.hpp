/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef TEST_FUNCTIONS_HPP_
#define TEST_FUNCTIONS_HPP_

#include "CoreConstants.hpp"
#include "CoreGlobals.hpp"
#include "Model.hpp"

Implementation::Unit getFirstUnit(ModelPtr model);

Implementation::Model* createBaseModel(
    int bacteria = 0,
    int teams = 1
);

#endif
