/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "Model.hpp"

typedef int (Implementation::Model::*IntOneArgMethod) (
    const Abstract::Point& coordinates
) const;

typedef void (Implementation::Model::*OneArgMethod) (
    const Abstract::Point& coordinates
);

typedef void (Implementation::Model::*TwoArgsMethod) (
    const Abstract::Point& coordinates,
    int change
);

typedef void (Implementation::Model::*MultiArgsMethod) (
    const Abstract::Point& coordinates,
    int mass,
    int direction,
    int team,
    int instruction
);

template<typename Func>
static void checkOneArgMethod(
    Implementation::Model* model,
    Func model_method,
    int arg1,
    int arg2
) {
    Abstract::Point coordinates(arg1, arg2);
    BOOST_REQUIRE_THROW(
        ((*model).*model_method)(coordinates), Exception
    );
}

template<typename Func>
void checkModelMethodForThrow(
    Implementation::Model* model,
    Func model_method,
    int arg1,
    int arg2
) {
    BOOST_REQUIRE_THROW(
        ((*model).*model_method)(arg1, arg2), Exception
    );
}

template<>
void checkModelMethodForThrow<IntOneArgMethod>(
    Implementation::Model* model,
    IntOneArgMethod model_method,
    int arg1,
    int arg2
) {
    checkOneArgMethod(model, model_method, arg1, arg2);
}

template<>
void checkModelMethodForThrow<OneArgMethod>(
    Implementation::Model* model,
    OneArgMethod model_method,
    int arg1,
    int arg2
) {
    checkOneArgMethod(model, model_method, arg1, arg2);
}

template<>
void checkModelMethodForThrow<TwoArgsMethod>(
    Implementation::Model* model,
    TwoArgsMethod model_method,
    int arg1,
    int arg2
) {
    Abstract::Point coordinates(arg1, arg2);
    BOOST_REQUIRE_THROW(
        ((*model).*model_method)(coordinates, 0), Exception
    );
}

template<>
void checkModelMethodForThrow<MultiArgsMethod>(
    Implementation::Model* model,
    MultiArgsMethod model_method,
    int arg1,
    int arg2
) {
    Abstract::Point coordinates(arg1, arg2);
    BOOST_REQUIRE_THROW(
        ((*model).*model_method)(
            coordinates,
            DEFAULT_MASS,
            0,
            0,
            0
        ),
        Exception
    );
}

// "dead" test: attempt to do something with dead bacterium
template<typename Func>
static void deadTest(
    Implementation::Model* model,
    Func model_method
) {
    model->kill(0, 0);
    checkModelMethodForThrow(
        model,
        model_method,
        0,
        0
    );
}

template<typename Func>
static void checkErrorHandling(
    Implementation::Model* model,
    Func model_method,
    bool dead_test
) {
    // Range errors: test all combinations of
    // "wrong" (outside of correct range) arguments.
    // This solution works for coordinates and non-coordinates
    // methods of Model. (< 0, = 0, > MAX)
    int wrong_args[] = {-1, 0, MIN_WIDTH};
    BOOST_FOREACH (int arg1, wrong_args) {
        BOOST_FOREACH (int arg2, wrong_args) {
            if ((arg1 != 0) || (arg2 != 0)) {
                // (0, 0) is correct
                checkModelMethodForThrow(
                    model,
                    model_method,
                    arg1,
                    arg2
                );
            }
        }
    }
    if (dead_test) {
        // "dead" error
        // (attempt to do something with dead bacterium)
        deadTest(model, model_method);
    }
}

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

BOOST_AUTO_TEST_CASE (get_team_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    int team = model->getTeamByCoordinates(coordinates);
    BOOST_REQUIRE(team == 0);
    checkErrorHandling<IntOneArgMethod>(
        model,
        &Implementation::Model::getTeamByCoordinates,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (width_test) {
    Implementation::Model* model = createBaseModel();
    BOOST_REQUIRE(model->getWidth() == MIN_WIDTH);
    delete model;
}

BOOST_AUTO_TEST_CASE (height_test) {
    Implementation::Model* model = createBaseModel();
    BOOST_REQUIRE(model->getHeight() == MIN_HEIGHT);
    delete model;
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

BOOST_AUTO_TEST_CASE (is_alive_test) {
    Implementation::Model* model = createBaseModel(1, 1);
    checkErrorHandling(
        model,
        &Implementation::Model::isAlive,
        false
    );
    BOOST_REQUIRE(model->isAlive(0, 0) == true);
    model->kill(0, 0);
    BOOST_REQUIRE(model->isAlive(0, 0) == false);
    delete model;
}

BOOST_AUTO_TEST_CASE (get_instruction_test) {
    Implementation::Model* model = createBaseModel(1, 1);
    int instruction = model->getInstruction(0, 0);
    BOOST_REQUIRE(instruction == 0);
    checkErrorHandling(
        model,
        &Implementation::Model::getInstruction,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (get_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    Abstract::Point derived_coordinates = model->getCoordinates(0, 0);
    BOOST_REQUIRE(derived_coordinates == coordinates);
    checkErrorHandling(
        model,
        &Implementation::Model::getCoordinates,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (get_direction_test) {
    Implementation::Model* model = createBaseModel();
    createInBaseCoordinates(model);
    int direction = model->getDirection(0, 0);
    BOOST_REQUIRE(direction == Abstract::LEFT);
    checkErrorHandling(
        model,
        &Implementation::Model::getDirection,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (get_mass_test) {
    Implementation::Model* model = createBaseModel(1, 1);
    int mass = model->getMass(0, 0);
    BOOST_REQUIRE(mass == DEFAULT_MASS);
    checkErrorHandling(
        model,
        &Implementation::Model::getMass,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (kill_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    model->kill(0, 0);
    Abstract::CellState state = model->cellState(coordinates);
    BOOST_REQUIRE(state == Abstract::EMPTY);
    BOOST_REQUIRE(model->isAlive(0, 0) == false);
    // error handling checks
    createInBaseCoordinates(model);
    // FIXME test doesn't work correctly without this function call.
    // The solution is to use set instead of vector in model.
    model->clearBeforeMove(0);
    checkErrorHandling(
        model,
        &Implementation::Model::kill,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (kill_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    model->killByCoordinates(coordinates);
    Abstract::CellState state = model->cellState(coordinates);
    BOOST_REQUIRE(state == Abstract::EMPTY);
    checkErrorHandling<OneArgMethod>(
        model,
        &Implementation::Model::killByCoordinates,
        false
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (change_mass_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    int test_val = 1;
    model->changeMassByCoordinates(coordinates, test_val);
    int new_mass = model->getMassByCoordinates(coordinates);
    BOOST_REQUIRE(new_mass == (DEFAULT_MASS + test_val));
    model->changeMassByCoordinates(coordinates, -test_val);
    new_mass = model->getMassByCoordinates(coordinates);
    BOOST_REQUIRE(new_mass == DEFAULT_MASS);
    checkErrorHandling<TwoArgsMethod>(
        model,
        &Implementation::Model::changeMassByCoordinates,
        true
    );
    delete model;
}

BOOST_AUTO_TEST_CASE (create_coordinates_test) {
    Implementation::Model* model = createBaseModel();
    Abstract::Point coordinates = createInBaseCoordinates(model);
    Abstract::CellState state = model->cellState(coordinates);
    BOOST_REQUIRE(state == Abstract::BACTERIUM);
    checkErrorHandling<MultiArgsMethod>(
        model,
        &Implementation::Model::createNewByCoordinates,
        false
    );
    delete model;
}
