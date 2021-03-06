/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef CREATOR_HPP_
#define CREATOR_HPP_

#include <QtCore>

#include <string>
#include <fstream>
#include <vector>

#include "CoreGlobals.hpp"
#include "CoreConstants.hpp"
#include "Interpreter.hpp"
#include "Model.hpp"
#include "Changer.hpp"

class Creator {
public:
    static ModelPtr createSpecialModel(
        int width,
        int height,
        int teams,
        const Units& units
    );

    static ModelPtr createModel(
        int width,
        int height,
        int bacteria,
        int teams
    );

    static InterpreterPtr createCoreObjects(
        ModelPtr model,
        const QStringList& script_names,
        ChangerPtrs& changers
    );
};

#endif
