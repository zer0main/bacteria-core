/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <fstream>

#include <QStringList>

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

BOOST_AUTO_TEST_CASE (create_special_model_test) {
    Implementation::Unit unit(
        Abstract::Point(0, 0),
        DEFAULT_MASS,
        0,
        0,
        0
    );
    Units units(1, unit);
    ModelPtr model = Creator::createSpecialModel(
        MIN_WIDTH,
        MIN_HEIGHT,
        1,
        units
    );
    baseModelChecks(model, MIN_WIDTH, MIN_HEIGHT, 1);
    BOOST_REQUIRE(model->getCoordinates(0, 0) == unit.coordinates);
    BOOST_REQUIRE(model->getMass(0, 0) == unit.mass);
    BOOST_REQUIRE(model->getDirection(0, 0) == unit.direction);
    BOOST_REQUIRE(model->getInstruction(0, 0) == unit.instruction);
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

BOOST_AUTO_TEST_CASE (create_core_objects_test) {
    ModelPtr model = Creator::createModel(
        MIN_WIDTH,
        MIN_HEIGHT,
        1,
        1
    );
    ChangerPtrs changers;
    std::ofstream bact_file("eat.bact");
    if (bact_file.is_open()) {
        bact_file << "eat\n";
        bact_file.close();
    } else {
        std::cerr << "Error: Unable to open script file." << std::endl;
    }
    QStringList scripts(QString("eat.bact"));
    InterpreterPtr interpreter = Creator::createCoreObjects(
        model,
        scripts,
        changers
    );
    BOOST_REQUIRE(changers[0]->getBacteriaNumber() == 1);
    interpreter->makeMove(*(changers[0].data()), 0);
    BOOST_REQUIRE(model->getMass(0, 0) == DEFAULT_MASS + EAT_MASS);
}
