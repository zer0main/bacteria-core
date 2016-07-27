/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Creator.hpp"

ModelPtr Creator::createSpecialModel(
    int width,
    int height,
    int teams,
    const Units& units
) const {
    ModelPtr model = createModel(width, height, 0, teams);
    foreach (Implementation::Unit unit, units) {
        model->createNewByCoordinates(
            unit.coordinates,
            unit.mass,
            unit.direction,
            unit.team,
            unit.instruction
        );
    }
    return model;
}

ModelPtr Creator::createModel(
    int width,
    int height,
    int bacteria,
    int teams
) const {
    ModelPtr model =
        ModelPtr(Abstract::makeModel<Implementation::Model>(
            width,
            height,
            bacteria,
            teams
        ));
    return model;
}

InterpreterPtr Creator::createCoreObjects(
    ModelPtr model,
    const QStringList& script_names,
    ChangerPtrs& changers
) const {
    Strings script_strs;
    for (int i = 0; i < script_names.size(); i++) {
        std::ifstream script(script_names[i].toStdString().c_str());
        std::string script_str;
        int instructions = 0;
        for (std::string buffer; getline(script, buffer); ) {
            script_str += buffer + '\n';
            instructions++;
        }
        ChangerPtr changer = ChangerPtr(
            new Implementation::Changer(model, i, 0, instructions)
        );
        changers.push_back(changer);
        script_strs.push_back(script_str);
        script.close();
    }
    InterpreterPtr interpreter = InterpreterPtr(
        new Implementation::Interpreter()
    );
    interpreter->makeBytecode(script_strs);
    return interpreter;
}
