/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "CoreConstants.hpp"
#include "CoreGlobals.hpp"
#include "Model.hpp"
#include "Changer.hpp"
#include "Interpreter.hpp"
#include "State.hpp"

BOOST_AUTO_TEST_CASE (Interpreter_test) {
    Implementation::Interpreter interpreter;
    ModelPtr model =
        ModelPtr(
            Abstract::makeModel<Implementation::Model>(
                MIN_WIDTH,
                MIN_HEIGHT,
                1,
                1
            )
        );
    Implementation::Unit unit(
        model->getCoordinates(0, 0),
        model->getMass(0, 0),
        model->getDirection(0, 0),
        0,
        0
    );
    Implementation::Changer changer(model, 0, 0, 1);
    Abstract::State state;
    Strings code;
    code.push_back("eat\n");
    interpreter.makeBytecode(code);
    interpreter.makeMove(changer, &state);
    BOOST_REQUIRE(model->getBacteriaNumber(0) == 1);
    BOOST_REQUIRE(model->getCoordinates(0, 0) == unit.coordinates);
    BOOST_REQUIRE(model->getMass(0, 0) == unit.mass + EAT_MASS);
    BOOST_REQUIRE(model->getDirection(0, 0) == unit.direction);
    BOOST_REQUIRE(model->getInstruction(0, 0) == unit.instruction);
}
