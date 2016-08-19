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

BOOST_AUTO_TEST_CASE (Bytecode_exceptions_test) {
    std::string invalid_spec("eat grgr\n");
    BOOST_REQUIRE_THROW(
        Implementation::Bytecode::make(invalid_spec),
        Exception
    );
    std::string too_many_funcs("eat go\n");
    BOOST_REQUIRE_THROW(
        Implementation::Bytecode::make(too_many_funcs),
        Exception
    );
    std::string invalid_args("eat 10 10\n");
    BOOST_REQUIRE_THROW(
        Implementation::Bytecode::make(invalid_args),
        Exception
    );
    std::string correct_code("eat\n");
    BytecodePtr bytecode = Implementation::Bytecode::make(
        correct_code
    );
    BOOST_REQUIRE_THROW(bytecode->getInstruction(-1), Exception);
    BOOST_REQUIRE_THROW(bytecode->getInstruction(1), Exception);
}

BOOST_AUTO_TEST_CASE (Bytecode_test) {
    std::string code("clon\ngo r\nstr 5\njl 5 10\n");
    BytecodePtr bytecode = Implementation::Bytecode::make(code);
    PackedInstructions packed_inst;
    // Non-argument functions, functions with one argument,
    // functions with two arguments and functions
    // with spec argument
    int function_types = 4;
    for (int i = 0; i < function_types; i++) {
        packed_inst.push_back(bytecode->getInstruction(i));
    }
    checkPackedInstruction(bytecode->getInstruction(0), 2, -1, -1, false);
    checkPackedInstruction(bytecode->getInstruction(1), 1, -1, -1, true);
    checkPackedInstruction(bytecode->getInstruction(2), 3, 5, -1, false);
    checkPackedInstruction(bytecode->getInstruction(3), 9, 5, 10, false);
}
