/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef BYTECODE_HPP_
#define BYTECODE_HPP_


namespace Implementation {

enum Type {
    FUNCTION,
    PARAMETER,
    SPECIFICATION,
    DELIMITER
};

struct Token {
    Token(
        Type type,
        int parameter = -1,
        bool spec = false,
        int function_id = -1
    );

    Type type;
    int parameter;
    int function_id;
    bool spec;
};

}

#endif
