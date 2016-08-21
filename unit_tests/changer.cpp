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

BOOST_AUTO_TEST_CASE (changer_get_instruction_test) {
    ModelPtr model(createBaseModel(1, 1));
    // total number of instructions is 2
    Implementation::Changer changer(model, 0, 0, 2);
    // initial state
    BOOST_REQUIRE(changer.getInstruction(0) == 0);
    Abstract::Params params(-1, -1, false);
    changer.eat(&params, 0);
    // after execution of the first command
    BOOST_REQUIRE(changer.getInstruction(0) == 1);
    int commands = 5;
    params = Abstract::Params(commands, -1, false);
    for (int i = 0; i < commands; i++) {
        // it must execute the same command `commands` times
        BOOST_REQUIRE(changer.getInstruction(0) == 1);
        changer.clearBeforeMove();
        changer.eat(&params, 0);
    }
    // cycle: after execution of all commands it returns to initial state
    BOOST_REQUIRE(changer.getInstruction(0) == 0);
}
