/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "Model.hpp"

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
void checkModelMethodForThrow<OneArgMethod>(
    Implementation::Model* model,
    OneArgMethod model_method,
    int arg1,
    int arg2
) {
    Abstract::Point coordinates(arg1, arg2);
    BOOST_REQUIRE_THROW(
        ((*model).*model_method)(coordinates), Exception
    );
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
    Func model_method,
    bool by_coordinates
) {
    if (by_coordinates) {
        Abstract::Point coordinates(0, 0);
        model->killByCoordinates(coordinates);
    } else {
        model->kill(0, 0);
    }
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
    bool by_coordinates,
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
        deadTest(model, model_method, by_coordinates);
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
    checkErrorHandling(
        model,
        &Implementation::Model::getMass,
        false,
        true
    );
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
    checkErrorHandling(
        model,
        &Implementation::Model::kill,
        false,
        true
    );
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
