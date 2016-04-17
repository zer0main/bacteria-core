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

struct Params {
    Params(int p1, int p2, bool spec);

    int p1, p2;
    bool spec;
};

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
    Ints remaining_actions_;
    Ints remaining_pseudo_actions_;
    // command = action OR pseudo action
    Ints completed_commands_;

    int team_;
    int move_number_;
};

}

#endif
