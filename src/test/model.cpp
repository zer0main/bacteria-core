/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "Model.hpp"

static Abstract::Point createInBaseCoordinates(
    Implementation::Model* model
) {
    Abstract::Point coordinates(0, 0);
    model->createNewByCoordinates(
        coordinates,
        DEFAULT_MASS,
        0,
        0,
        0
    );
    return coordinates;
}

static Implementation::Model* createBaseModel(
    int bacteria = 0,
    int teams = 1
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

BOOST_AUTO_TEST_CASE (bacteria_number_test) {
    Implementation::Model* model = createBaseModel();
    int bacteria_number = model->getBacteriaNumber(0);
    BOOST_REQUIRE(bacteria_number == 0);
    createInBaseCoordinates(model);
    bacteria_number = model->getBacteriaNumber(0);
    BOOST_REQUIRE(bacteria_number == 1);
    delete model;
}

BOOST_AUTO_TEST_CASE (get_mass_test) {
    Implementation::Model* model = createBaseModel(1, 1);
    int mass = model->getMass(0, 0);
    BOOST_REQUIRE(mass == DEFAULT_MASS);
    delete model;
}

BOOST_AUTO_TEST_CASE (height_test) {
    Implementation::Model* model = createBaseModel();
    BOOST_REQUIRE(model->getHeight() == MIN_HEIGHT);
    delete model;
}

BOOST_AUTO_TEST_CASE (width_test) {
    Implementation::Model* model = createBaseModel();
    BOOST_REQUIRE(model->getWidth() == MIN_WIDTH);
    delete model;
}

BOOST_AUTO_TEST_CASE (kill_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    model->kill(0, 0);
    Abstract::CellState state = model->cellState(coordinates);
    BOOST_REQUIRE(state == Abstract::EMPTY);
    delete model;
}

BOOST_AUTO_TEST_CASE (create_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    Abstract::CellState state = model->cellState(coordinates);
    BOOST_REQUIRE(state == Abstract::BACTERIUM);
    delete model;
}

BOOST_AUTO_TEST_CASE (kill_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    model->killByCoordinates(coordinates);
    Abstract::CellState state = model->cellState(coordinates);
    BOOST_REQUIRE(state == Abstract::EMPTY);
    delete model;
}
