/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <stdexcept>

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

typedef void (Abstract::Changer::*ChangerMethod) (
    const Abstract::Params* params,
    int b_index
);

static const ChangerMethod CHANGER_FUNCTIONS[] = {
    &Abstract::Changer::eat,
    &Abstract::Changer::go,
    &Abstract::Changer::clon,
    &Abstract::Changer::str,
    &Abstract::Changer::left,
    &Abstract::Changer::right,
    &Abstract::Changer::back,
    &Abstract::Changer::turn,
    &Abstract::Changer::jg,
    &Abstract::Changer::jl,
    &Abstract::Changer::j,
    &Abstract::Changer::je
};

const size_t FUNCTION_COUNT =
    sizeof(CHANGER_FUNCTIONS) / sizeof(ChangerMethod);

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
    changer.clearBeforeMove();
    int bacteria = changer.getBacteriaNumber();
    int team = changer.getTeam();
    for (int b = 0; b < bacteria; b++) {
        while (!changer.endOfMove(b)) {
            int instruction_number = changer.getInstruction(b);
            PackedInstruction pi =
                bytecode_[team]->getInstruction(instruction_number);
            int func_id = pi.function_id;
            Abstract::Params params(pi.p1, pi.p2, pi.spec);
            if (func_id < 0 || func_id >= FUNCTION_COUNT) {
                throw std::range_error("Bad opcode");
            }
            ChangerMethod func = CHANGER_FUNCTIONS[func_id];
            (changer.*func)(&params, b);
        }
    }
}

Abstract::State* Interpreter::createState_impl() const {
}

}
