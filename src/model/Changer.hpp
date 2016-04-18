/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef CHANGER_HPP_
#define CHANGER_HPP_

#include "CoreConstants.hpp"
#include "CoreGlobals.hpp"
#include "Model.hpp"
#include "Exception.hpp"
#include "random.hpp"

namespace Abstract {

struct Params {
    Params(int p1, int p2, bool spec);

    int p1, p2;
    bool spec;
};

class Changer {
public:
    bool endOfMove(int bacterium_index) const;

    int getBacteriaNumber() const;

    int getTeam() const;

protected:
    Changer(Model& model, int team, int move_number);

    virtual bool endOfMove_impl(int bacterium_index) const = 0;

    virtual int getBacteriaNumber_impl() const = 0;

    virtual int getTeam_impl() const = 0;

};

}

namespace Implementation {

class Changer : public Abstract::Changer {
public:
    Changer(Abstract::Model& model, int team, int move_number);

protected:
    bool endOfMove_impl(int bacterium_index) const;

    int getBacteriaNumber_impl() const;

    int getTeam_impl() const;

private:
    Abstract::Model& model_;
    Ints remaining_actions_;
    Ints remaining_pseudo_actions_;
    // command = action OR pseudo action
    Ints completed_commands_;

    int team_;
    int move_number_;
};

}

#endif
