/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef CHANGER_HPP_
#define CHANGER_HPP_

#include "Model.hpp"

namespace Abstract {

class Changer {
protected:
    Changer(Model& model, int team, int move_number);
};

}

#endif
