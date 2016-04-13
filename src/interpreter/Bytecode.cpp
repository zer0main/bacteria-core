/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Bytecode.hpp"

namespace Implementation {

// These two static arrays describe language: all possible
// functions ans specifications.

static const char* const specifications_registry[] = {
    "r"
};

static const char* const functions_registry[] = {
    "eat", "go", "clon", "str", "left", "right", "back", "turn",
    "jg", "jl", "j", "je", "jfe", "jw", "jfw", "jb", "jfb"
};

// These four static arrays contain IDs of functions from
// functions_registry.

static const int two_parameter_functions[] = {
    8, 9
};

static const int one_parameter_functions[] = {
    0, 1, 3, 4, 5, 10, 11, 12, 13, 14, 15, 16
};

static const int spec_functions[] = {
    0, 1, 7
};

static const int non_argument_functions[] = {
    0, 1, 2, 3, 4, 5, 6
};

static void split(
    const std::string& str,
    char delimiter,
    Strings items
) {
    std::stringstream s_stream(str);
    std::string item;
    while (std::getline(s_stream, item, delimiter)) {
        if (!item.empty()) {
            items.push_back(item);
        }
    }
}

Token::Token(
    Type type,
    int parameter,
    bool spec,
    int function_id
)
    : type(type)
    , parameter(parameter)
    , spec(spec)
    , function_id(function_id)
{
}

Instruction::Instruction(
    Token function,
    Token p1,
    Token p2,
    Token spec
)
    : function(function)
    , p1(p1)
    , p2(p2)
    , spec(spec)
{
}

PackedInstruction::PackedInstruction(
    int function_id,
    int p1,
    int p2,
    bool spec
)
    : function_id(function_id)
    , p1(p1)
    , p2(p2)
    , spec(spec)
{
}

}
