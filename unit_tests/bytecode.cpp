/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include "CoreGlobals.hpp"
#include "Bytecode.hpp"

static void checkPackedInstruction(
    Implementation::PackedInstruction pi,
    int function_id,
    int p1,
    int p2,
    bool spec
) {
    BOOST_REQUIRE(pi.function_id == function_id);
    BOOST_REQUIRE(pi.p1 == p1);
    BOOST_REQUIRE(pi.p2 == p2);
    BOOST_REQUIRE(pi.spec == spec);
}
