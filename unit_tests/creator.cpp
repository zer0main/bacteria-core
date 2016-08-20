/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "CoreConstants.hpp"
#include "CoreGlobals.hpp"
#include "Creator.hpp"

static void baseModelChecks(
    ModelPtr model,
    int width,
    int height,
    int bacteria_number
) {
    BOOST_REQUIRE(model->getWidth() == width);
    BOOST_REQUIRE(model->getHeight() == height);
    BOOST_REQUIRE(model->getBacteriaNumber(0) == bacteria_number);
}

BOOST_AUTO_TEST_CASE (create_model_test) {
    ModelPtr model = Creator::createModel(
        MIN_WIDTH,
        MIN_HEIGHT,
        1,
        1
    );
    baseModelChecks(model, MIN_WIDTH, MIN_HEIGHT, 1);
}
