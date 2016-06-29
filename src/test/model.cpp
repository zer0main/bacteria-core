/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "Model.hpp"

template<typename Func>
static void checkErrorHandling(
    Implementation::Model* model,
    Func model_method
) {
    // range errors: test all combinations of
    // "wrong" (outside of correct range) arguments
    // Max_index: 0; min_index: 0
    for (int arg1 = -1; arg1 <= 1; arg1++) {
        for (int arg2 = -1; arg2 <= 1; arg2++) {
            if ((arg1 != 0) || (arg2 != 0)) {
                // (0, 0) is correct
                BOOST_REQUIRE_THROW(
                    ((*model).*model_method)(arg1, arg2), Exception
                );
            }
        }
    }
    // "dead" error
    // (attempt to do something with dead bacterium)
    model->kill(0, 0);
    BOOST_REQUIRE_THROW(
        ((*model).*model_method)(0, 0), Exception
    );
}

#define CREATE_NEW \
    model->createNewByCoordinates( \
        coordinates, \
        DEFAULT_MASS, \
        0, \
        0, \
        0 \
    );

static Abstract::Point createInBaseCoordinates(
    Implementation::Model* model
) {
    Abstract::Point coordinates(0, 0);
    CREATE_NEW
    return coordinates;
}

static void createByCoordinates(
    Implementation::Model* model,
    Abstract::Point coordinates
) {
    CREATE_NEW
}

#undef CREATE_NEW

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
    // range errors
    BOOST_REQUIRE_THROW(model->getBacteriaNumber(-1), Exception);
    BOOST_REQUIRE_THROW(model->getBacteriaNumber(1), Exception);
    delete model;
}

BOOST_AUTO_TEST_CASE (get_mass_test) {
    Implementation::Model* model = createBaseModel(1, 1);
    int mass = model->getMass(0, 0);
    BOOST_REQUIRE(mass == DEFAULT_MASS);
    checkErrorHandling(model, &Implementation::Model::getMass);
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
    // error handling checks
    createInBaseCoordinates(model);
    // FIXME test doesn't work correctly without this function call.
    // The solution is to use set instead of vector in model.
    model->clearBeforeMove(0);
    checkErrorHandling(model, &Implementation::Model::kill);
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
