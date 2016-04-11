/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include <vector>
#include <sstream>
#include <string>

#include "CoreGlobals.hpp"
#include "State.hpp"
#include "Changer.hpp"

namespace Abstract {

class Interpreter {
public:
    void makeMove(Changer& changer, State* st) const;

    State* createState() const;

protected:
    Interpreter(const Strings& scripts);

    virtual void makeMove_impl(Changer& changer, State* st) const = 0;

    virtual State* createState_impl() const = 0;
};

}

namespace Implementation {

struct Instruction {
    const char* command;
    Ints parameters;
};

class Interpreter : public Abstract::Interpreter {
public:
    Interpreter(const Strings& scripts);

protected:
    void makeMove_impl(
        Abstract::Changer& changer,
        Abstract::State* st
    ) const;

    Abstract::State* createState_impl() const;
};

}

#endif
