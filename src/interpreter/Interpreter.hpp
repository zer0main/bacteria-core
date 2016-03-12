/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include <vector>
#include <string>

#include "State.hpp"
#include "Changer.hpp"

namespace Abstract {

typedef std::vector<std::string> Strings;

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

#endif
