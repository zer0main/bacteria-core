/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Bytecode.hpp"

namespace Implementation {

// These two static arrays describe language: all possible
// functions ans specifications.

static const char* const specifications_registry[] = {
    "r"
};

static const char* const functions_registry[] = {
    "eat", "go", "clon", "str", "left", "right", "back", "turn",
    "jg", "jl", "j", "je", /*"jfe", "jw", "jfw",
    "jb", "jfb"*/
};

// These four static arrays contain IDs of functions from
// functions_registry.

static const int two_parameter_functions[] = {
    8, 9
};

static const int one_parameter_functions[] = {
    0, 1, 3, 4, 5, 10, 11, 12, 13, 14, 15, 16
};

static const int spec_functions[] = {
    0, 1, 7
};

static const int non_argument_functions[] = {
    0, 1, 2, 3, 4, 5, 6
};

static void split(
    const std::string& str,
    char delimiter,
    Strings& items
) {
    std::stringstream s_stream(str);
    std::string item;
    while (std::getline(s_stream, item, delimiter)) {
        if (!item.empty()) {
            items.push_back(item);
        }
    }
}

static int isParameter(const std::string& what) {
    bool is_parameter = true;
    for (int i = 0; i < what.size(); i++) {
        if (!isdigit(what[i])) {
            is_parameter = false;
        }
    }
    return (is_parameter) ? atoi(what.c_str()) : -1;
}

static bool isSpecification(const std::string& what) {
    size_t el_size = sizeof(char*);
    size_t size = sizeof(specifications_registry) / el_size;
    for (int i = 0; i < size; i++) {
        if (what.compare(specifications_registry[i]) == 0) {
            return true;
        }
    }
    return false;
}

static int isFunction(const std::string& what) {
    size_t el_size = sizeof(char*);
    size_t size = sizeof(functions_registry) / el_size;
    for (int i = 0; i < size; i++) {
        if (what.compare(functions_registry[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static Token makeToken(const std::string& token_str) {
    Type type;
    if (token_str == "\n") {
        type = DELIMITER;
    } else if (isParameter(token_str) != -1) {
        type = PARAMETER;
    } else if (isSpecification(token_str)) {
        type = SPECIFICATION;
    } else if (isFunction(token_str) != -1) {
        type = FUNCTION;
    } else {
        throw Exception("Interpreter: invalid token");
    }
    Token token(
        type,
        isParameter(token_str),
        isSpecification(token_str),
        isFunction(token_str)
    );
    return token;
}

static void checkFunctions(const Token& first, int funcs) {
    if ((first.type != FUNCTION) || (funcs != 1)) {
        throw Exception("Interpreter: function is missed "
                        "/ it's in the incorrect position "
                        "/ there are too many functions "
                        "for one command.");
    }
}

static bool searchId(int id, const int* array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == id) {
            return true;
        }
    }
    return false;
}

static void checkById(int id, int params, int specs) {
    bool ok = false;
    if ((params == 2) && (specs == 0)) {
        int size = sizeof(two_parameter_functions) / sizeof(int);
        ok = searchId(id, two_parameter_functions, size);
    } else if ((params == 1) && (specs == 0)) {
        int size = sizeof(one_parameter_functions) / sizeof(int);
        ok = searchId(id, one_parameter_functions, size);
    } else if ((params == 0) && (specs == 1)) {
        int size = sizeof(spec_functions) / sizeof(int);
        ok = searchId(id, spec_functions, size);
    } else if ((params == 0) && (specs == 0)) {
        int size = sizeof(non_argument_functions) / sizeof(int);
        ok = searchId(id, non_argument_functions, size);
    }
    if (!ok) {
        throw Exception("Interpreter: invalid arguments of function");
    }
}

static Instruction makeInstruction(
    int params,
    int specs,
    const Tokens& tokens_group
) {
    Token function = tokens_group[0];
    Token p1(PARAMETER);
    Token p2(PARAMETER);
    Token spec(SPECIFICATION);
    if (params == 2) {
        p1 = tokens_group[1];
        p2 = tokens_group[2];
    } else if (params == 1) {
        p1 = tokens_group[1];
    } else if (specs == 1) {
        spec = tokens_group[1];
    }
    Instruction instruction(function, p1, p2, spec);
    return instruction;
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

BytecodePtr Bytecode::make(const std::string& source) {
    BytecodePtr bytecode(new Bytecode);
    bytecode->generateBytecode(source);
    return bytecode;
}

PackedInstruction Bytecode::getInstruction(int index) const {
    bool less = index < 0;
    bool greater = index >= bytecode_.size();
    if (less || greater) {
        throw Exception("Bytecode: invalid index of instruction.");
    }
    return bytecode_[index];
}

void Bytecode::generateBytecode(const std::string& source) {
    Tokens tokens = lexer(source);
    Instructions ast = parser(tokens);
    compile(ast);
}

Tokens Bytecode::lexer(const std::string& source) const {
    Strings lines;
    split(source, '\n', lines);
    Strings tokens_str;
    for (int i = 0; i < lines.size(); i++) {
        split(lines[i], ' ', tokens_str);
        tokens_str.push_back("\n");
    }
    Tokens result;
    for (int i = 0; i < tokens_str.size(); i++) {
        result.push_back(makeToken(tokens_str[i]));
    }
    return result;
}

Instructions Bytecode::parser(const Tokens& tokens) const {
    Tokens tokens_group;
    // Abstract syntax tree.
    Instructions ast;
    int funcs = 0, params = 0, specs = 0;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == DELIMITER) {
            checkFunctions(tokens_group[0], funcs);
            checkById(tokens_group[0].function_id, params, specs);
            Instruction inst =
                makeInstruction(params, specs, tokens_group);
            ast.push_back(inst);
            funcs = 0, params = 0, specs = 0;
            tokens_group.clear();
        } else {
            if (tokens[i].type == FUNCTION) {
                funcs++;
            } else if (tokens[i].type == PARAMETER) {
                params++;
            } else {
                specs++;
            }
            tokens_group.push_back(tokens[i]);
        }
    }
    return ast;
}

void Bytecode::compile(const Instructions& instructions) {
    for (int i = 0; i < instructions.size(); i++) {
        int func_id = instructions[i].function.function_id;
        int p1 = instructions[i].p1.parameter;
        int p2 = instructions[i].p2.parameter;
        bool spec = instructions[i].spec.spec;
        PackedInstruction packed_inst(func_id, p1, p2, spec);
        bytecode_.push_back(packed_inst);
    }
}

}
