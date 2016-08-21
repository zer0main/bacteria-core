/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "TestFunctions.hpp"

#include "CoreConstants.hpp"
#include "CoreGlobals.hpp"
#include "Model.hpp"
#include "Changer.hpp"

BOOST_AUTO_TEST_CASE (get_bacteria_number_test) {
    ModelPtr model(createBaseModel(1, 1));
    Implementation::Changer changer(model, 0, 0, 0);
    BOOST_REQUIRE(changer.getBacteriaNumber() == 1);
}

BOOST_AUTO_TEST_CASE (get_team_test) {
    ModelPtr model(createBaseModel(1, 1));
    Implementation::Changer changer(model, 0, 0, 0);
    BOOST_REQUIRE(changer.getTeam() == 0);
}
