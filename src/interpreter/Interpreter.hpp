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
#include "Bytecode.hpp"
#include "State.hpp"
#include "Changer.hpp"

namespace Abstract {

class Interpreter {
public:
    Interpreter();

    virtual ~Interpreter();

    void makeBytecode(const Strings& scripts);

    void makeMove(Changer& changer, State* st) const;

    State* createState() const;

protected:
    virtual void makeBytecode_impl(const Strings& scripts) = 0;

    virtual void makeMove_impl(Changer& changer, State* st) const = 0;

    virtual State* createState_impl() const = 0;
};

}

namespace Implementation {

class Interpreter : public Abstract::Interpreter {
protected:
    void makeBytecode_impl(const Strings& scripts);

    void makeMove_impl(
        Abstract::Changer& changer,
        Abstract::State* st
    ) const;

    Abstract::State* createState_impl() const;

private:
    BytecodePtrs bytecode_;
};

}

#endif
