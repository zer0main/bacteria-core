/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/foreach.hpp>

#include <iostream>
#include <string>

#include "YamlReader.hpp"
#include "YamlEmitter.hpp"

#include "CoreGlobals.hpp"
#include "Creator.hpp"
#include "Model.hpp"
#include "Changer.hpp"
#include "Interpreter.hpp"
#include "Exception.hpp"

static Units getUnits(const ModelPtr& model, int teams) {
    Units units;
    for (int team = 0; team < teams; team++) {
        for (int b = 0; b < model->getBacteriaNumber(team); b++) {
            Implementation::Unit unit(
                model->getCoordinates(team, b),
                model->getMass(team, b),
                model->getDirection(team, b),
                team,
                model->getInstruction(team, b)
            );
            units.push_back(unit);
        }
    }
    return units;
}

static void writeResults(
    const std::string& output_yaml,
    ModelPtr& model,
    int teams
) {
    YamlEmitter emitter;
    emitter.writeWidth(model->getWidth());
    emitter.writeHeight(model->getHeight());
    Units units = getUnits(model, teams);
    emitter.writeUnits(units);
    std::ofstream out_file(output_yaml.c_str());
    out_file << emitter.out_;
}

static void runTest(
    ModelPtr& model,
    ChangerPtrs& changers,
    InterpreterPtr& interpreter,
    int moves_number
) {
    for (int move = 0; move < moves_number; move++) {
        BOOST_FOREACH (ChangerPtr changer, changers) {
            interpreter->makeMove(*(changer.data()), 0);
        }
    }
}

// Return number of moves
static int createEverything(
    const std::string& input_yaml,
    const Strings& scripts,
    ModelPtr& model,
    ChangerPtrs& changers,
    InterpreterPtr& interpreter
) {
    YamlReader reader(input_yaml);
    int width = reader.getWidth();
    int height = reader.getHeight();
    int moves_number = reader.getMovesNumber();
    Units units = reader.getUnits();
    Creator creator;
    model = creator.createSpecialModel(
        width,
        height,
        scripts.size(),
        units
    );
    interpreter = creator.createCoreObjects(
        model,
        scripts,
        changers
    );
    return moves_number;
}

static void argParse(
    int argc,
    char* argv[],
    std::string& input_yaml,
    std::string& output_yaml,
    Strings& scripts
) {
    // input yaml, script and output yaml
    int min_args_number = 4;
    if (argc < min_args_number) {
        throw Exception(
            "Integration tests: you must specify input yaml, "
            "script(s) and output yaml in args."
        );
    }
    input_yaml = argv[1];
    for (int i = 2; i < argc - 1; i++) {
        scripts.push_back(argv[i]);
    }
    output_yaml = argv[argc - 1];
}

int main(int argc, char* argv[]) {
    std::string input_yaml, output_yaml;
    Strings scripts;
    argParse(
        argc,
        argv,
        input_yaml,
        output_yaml,
        scripts
    );
    ModelPtr model;
    ChangerPtrs changers;
    InterpreterPtr interpreter;
    int moves_number = createEverything(
        input_yaml,
        scripts,
        model,
        changers,
        interpreter
    );
    runTest(model, changers, interpreter, moves_number);
    writeResults(output_yaml, model, changers.size());
    return 0;
}
