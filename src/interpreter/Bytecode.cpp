/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Bytecode.hpp"

namespace Implementation {


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
