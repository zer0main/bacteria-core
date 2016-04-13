/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef BYTECODE_HPP_
#define BYTECODE_HPP_

#include <cstdlib>
#include <vector>
#include <sstream>
#include <string>

#include <QtCore>

#include "Exception.hpp"
#include "CoreGlobals.hpp"

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

struct Instruction {
    Instruction(
        Token function,
        Token p1,
        Token p2,
        Token spec
    );

    Token function;
    // parameters
    Token p1, p2;
    Token spec;
};

struct PackedInstruction {
    PackedInstruction(
        int function_id,
        int p1,
        int p2,
        bool spec
    );

    int function_id;
    // parameters
    int p1, p2;
    bool spec;
};

class Bytecode {
public:
    static BytecodePtr make(const std::string& source);

private:
    PackedInstructions bytecode_;

    Bytecode();

    void generateBytecode(const std::string& source);

    Tokens lexer(const std::string& source) const;

    Instructions parser(const Tokens& tokens) const;

    void compile(const Instructions& instructions);
};

}

#endif
