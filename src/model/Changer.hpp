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
    void clearAfterMove();

    bool endOfMove(int bacterium_index) const;

    int getBacteriaNumber() const;

    int getTeam() const;

    int getInstruction(int bacterium_index) const;

protected:
    Changer(
        Model& model,
        int team,
        int move_number,
        int instructions
    );

    virtual void clearAfterMove_impl() = 0;

    virtual bool endOfMove_impl(int bacterium_index) const = 0;

    virtual int getBacteriaNumber_impl() const = 0;

    virtual int getTeam_impl() const = 0;

    virtual int getInstruction_impl(int bacterium_index) const = 0;

};

}

namespace Implementation {

class LogicalChanger {
public:
    LogicalChanger(
        Abstract::Model& model,
        int team,
        int move_number
    );

    void eat(int bacterium_index);

private:
    Abstract::Model& model_;
    int team_;
    int move_number_;
};

typedef void (LogicalChanger::*LogicalMethod) (int bacterium_index);

struct RepeaterParams {
    RepeaterParams(
        int bacterium_index,
        int commands,
        Ints& remaining_commands_vect,
        LogicalMethod logic_function
    );

    int bacterium_index;
    int commands;
    Ints& remaining_commands_vect;
    LogicalMethod logic_function;
};

class Changer : public Abstract::Changer {
public:
    Changer(
        Abstract::Model& model,
        int team,
        int move_number,
        int instructions
    );

protected:
    void clearAfterMove_impl();

    bool endOfMove_impl(int bacterium_index) const;

    int getBacteriaNumber_impl() const;

    int getTeam_impl() const;

    int getInstruction_impl(int bacterium_index) const;

private:
    Abstract::Model& model_;
    Ints remaining_actions_;
    Ints remaining_pseudo_actions_;
    // command = action OR pseudo action
    Ints completed_commands_;

    int team_;
    int move_number_;
    int instructions_;

    LogicalChanger logical_changer_;

    bool remainingActionsDecrement(
        Ints& remaining_commands_vect,
        int bacterium_index
    );

    void updateInstruction(int bacterium_index);

    int checkCommandsNumber(int number) const;

    void repeater(RepeaterParams* params);
};

}

#endif
