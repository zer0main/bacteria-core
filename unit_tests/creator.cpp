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

BOOST_AUTO_TEST_CASE (create_model_test) {
    ModelPtr model = Creator::createModel(
        MIN_WIDTH,
        MIN_HEIGHT,
        1,
        1
    );
    BOOST_REQUIRE(model->getWidth() == MIN_WIDTH);
    BOOST_REQUIRE(model->getHeight() == MIN_HEIGHT);
    BOOST_REQUIRE(model->getBacteriaNumber(0) == 1);
}
