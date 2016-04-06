/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Interpreter.hpp"

namespace Abstract {

Interpreter::Interpreter(const Strings& scripts) {
}

void Interpreter::makeMove(Changer& changer, State* st) const {
    return makeMove_impl(changer, st);
}

State* Interpreter::createState() const {
    return createState_impl();
}

}

namespace Implementation {

Interpreter::Interpreter(
    const Strings& scripts
)
    : Abstract::Interpreter(scripts) {
}

void Interpreter::makeMove_impl(
    Abstract::Changer& changer,
    Abstract::State* st
) const {
}

Abstract::State* Interpreter::createState_impl() const {
}

}
