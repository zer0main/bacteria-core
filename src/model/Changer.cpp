/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Changer.hpp"

namespace Abstract {

Params::Params(
    int p1,
    int p2,
    bool spec
)
    : p1(p1)
    , p2(p2)
    , spec(spec)
{
}

Changer::Changer(
    Model& /*model*/,
    int /*team*/,
    int /*move_number*/
) {
}

}

namespace Implementation {

Changer::Changer(
    Model& model,
    int team,
    int move_number
)
    : Abstract::Changer(model, team, move_number)
    , model_(model)
    , team_(team)
    , move_number_(move_number) {
}

}
