/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Interpreter.hpp"

namespace Abstract {

Interpreter::Interpreter() {
}

void Interpreter::makeBytecode(const Strings& scripts) {
    return makeBytecode_impl(scripts);
}

void Interpreter::makeMove(Changer& changer, State* st) const {
    return makeMove_impl(changer, st);
}

State* Interpreter::createState() const {
    return createState_impl();
}

}

namespace Implementation {

void Interpreter::makeBytecode_impl(const Strings& scripts) {
    for (int i = 0; i < scripts.size(); i++) {
        BytecodePtr bytecode_ptr = Bytecode::make(scripts[i]);
        bytecode_.push_back(bytecode_ptr);
    }
}

void Interpreter::makeMove_impl(
    Abstract::Changer& changer,
    Abstract::State* st
) const {
}

Abstract::State* Interpreter::createState_impl() const {
}

}
