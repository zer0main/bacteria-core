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
#include "Exception.hpp"
#include "Model.hpp"
#include "Changer.hpp"
#include "Interpreter.hpp"

typedef void (CheckerFunc) (
    Implementation::Unit prev,
    Implementation::Unit curr,
    bool spec
);

static void eatChecker(
    Implementation::Unit prev,
    Implementation::Unit curr,
    bool spec
) {
    if (spec) {
        int max_mass = prev.mass + RANDOM_MAX_ACTIONS * EAT_MASS;
        BOOST_REQUIRE(curr.mass >= prev.mass && curr.mass < max_mass);
    } else {
        BOOST_REQUIRE(curr.mass == (prev.mass + EAT_MASS));
    }
}

static Implementation::Changer createChanger(
    ModelPtr model,
    bool spec,
    bool many_commands
) {
    int instructions;
    if (spec && many_commands) {
        // func; func r; func n
        instructions = 3;
    } else if (spec) {
        // func; func r
        instructions = 2;
    } else if (many_commands) {
        // func; func n
        instructions = 2;
    } else {
        // func
        instructions = 1;
    }
    return Implementation::Changer(model, 0, 0, instructions);
}

static void checkSpec(
    ModelPtr model,
    Implementation::Changer& changer,
    Implementation::ChangerMethod tested,
    CheckerFunc checker
) {
    Implementation::Unit prev = getFirstUnit(model);
    Abstract::Params params(-1, -1, true);
    changer.clearBeforeMove();
    (changer.*tested)(&params, 0);
    Implementation::Unit curr = getFirstUnit(model);
    checker(prev, curr, true);
}

static void checkManyCommands(
    ModelPtr model,
    Implementation::Changer& changer,
    Implementation::ChangerMethod tested,
    CheckerFunc checker
) {
    int commands = 5;
    Abstract::Params params(commands, -1, false);
    for (int i = 0; i < commands; i++) {
        Implementation::Unit prev = getFirstUnit(model);
        changer.clearBeforeMove();
        (changer.*tested)(&params, 0);
        Implementation::Unit curr = getFirstUnit(model);
        checker(prev, curr, false);
    }
    // range error
    params = Abstract::Params(10000, -1, false);
    changer.clearBeforeMove();
    BOOST_REQUIRE_THROW((changer.*tested)(&params, 0), Exception);
}

static void checkLogicalMethod(
    Implementation::ChangerMethod tested,
    CheckerFunc checker,
    bool spec,
    bool many_commands
) {
    ModelPtr model(createBaseModel(1, 1));
    Implementation::Changer changer =
        createChanger(model, spec, many_commands);
    Implementation::Unit prev = getFirstUnit(model);
    Abstract::Params params(-1, -1, false);
    (changer.*tested)(&params, 0);
    Implementation::Unit curr = getFirstUnit(model);
    checker(prev, curr, false);
    if (many_commands) {
        checkManyCommands(model, changer, tested, checker);
    }
    if (spec) {
        checkSpec(model, changer, tested, checker);
    }
}

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

BOOST_AUTO_TEST_CASE (eat_test) {
    ModelPtr model(createBaseModel(1, 1));
    // number of instructions is 3: eat; eat 5; eat r
    Implementation::Changer changer(model, 0, 0, 3);
    Abstract::Params params(-1, -1, false);
    // eat
    changer.eat(&params, 0);
    BOOST_REQUIRE(model->getMass(0, 0) == DEFAULT_MASS + EAT_MASS);
    int commands = 5;
    params = Abstract::Params(commands, -1, false);
    for (int i = 0; i < commands; i++) {
        int expected_mass = EAT_MASS * (i + 1) + DEFAULT_MASS;
        BOOST_REQUIRE(model->getMass(0, 0) == expected_mass);
        changer.clearBeforeMove();
        // eat 5
        changer.eat(&params, 0);
    }
    int prev_mass = model->getMass(0, 0);
    params = Abstract::Params(-1, -1, true);
    changer.clearBeforeMove();
    // eat r
    changer.eat(&params, 0);
    int curr_mass = model->getMass(0, 0);
    int max_mass = prev_mass + RANDOM_MAX_ACTIONS * EAT_MASS;
    BOOST_REQUIRE(curr_mass >= prev_mass && curr_mass < max_mass);
    // range error
    params = Abstract::Params(10000, -1, false);
    BOOST_REQUIRE_THROW(changer.eat(&params, 0), Exception);
}
