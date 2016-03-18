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

namespace Implementation {

class Changer : public Abstract::Changer {
public:
    Changer(Model& model, int team, int move_number);

private:
    Model& model_;

    int team_;
    int move_number_;
};

}

#endif
