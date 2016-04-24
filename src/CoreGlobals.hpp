/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef CORE_GLOBALS_HPP_
#define CORE_GLOBALS_HPP_

#include <vector>
#include <QtCore>

namespace Abstract {

class Interpreter;
class Model;

}

namespace Implementation {

class Bytecode;
struct Token;
struct Instruction;
struct PackedInstruction;

struct Unit;

}

typedef QSharedPointer<const Abstract::Model> ConstModelPtr;
typedef QSharedPointer<Abstract::Model> ModelPtr;
typedef QSharedPointer<Abstract::Interpreter> InterpreterPtr;

typedef QSharedPointer<Implementation::Bytecode> BytecodePtr;
typedef std::vector<BytecodePtr> BytecodePtrs;
typedef std::vector<Implementation::Token> Tokens;
typedef std::vector<Implementation::Instruction> Instructions;
typedef std::vector<Implementation::PackedInstruction> PackedInstructions;

typedef QSharedPointer<Implementation::Unit> UnitPtr;
typedef std::vector<UnitPtr> Units;
typedef std::vector<Units> Teams;

typedef std::vector<int> Ints;
typedef std::vector<std::string> Strings;

#endif
