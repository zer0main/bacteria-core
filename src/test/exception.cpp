/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "Exception.hpp"

BOOST_AUTO_TEST_CASE (Exception_test) {
    std::string error_message;
    try {
        throw Exception("Error message");
    } catch (Exception& e) {
        error_message = e.what();
    }
    BOOST_REQUIRE(error_message == "Error message");
}
