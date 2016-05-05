/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef CHANGER_HPP_
#define CHANGER_HPP_

#include <cmath>
#include <algorithm>

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
    void clearBeforeMove();

    bool endOfMove(int bacterium_index) const;

    int getBacteriaNumber() const;

    int getTeam() const;

    int getInstruction(int bacterium_index) const;

    void eat(const Params* params, int bacterium_index);

    void go(const Params* params, int bacterium_index);

    void clon(const Params* params, int bacterium_index);

    void str(const Params* params, int bacterium_index);

    void left(const Params* params, int bacterium_index);

    void right(const Params* params, int bacterium_index);

    void back(const Params* params, int bacterium_index);

    void turn(const Params* params, int bacterium_index);

    void jg(const Params* params, int bacterium_index);

    void jl(const Params* params, int bacterium_index);

    void j(const Params* params, int bacterium_index);

    void je(const Params* params, int bacterium_index);

protected:
    Changer(
        ModelPtr model,
        int team,
        int move_number,
        int instructions
    );

    virtual void clearBeforeMove_impl() = 0;

    virtual bool endOfMove_impl(int bacterium_index) const = 0;

    virtual int getBacteriaNumber_impl() const = 0;

    virtual int getTeam_impl() const = 0;

    virtual int getInstruction_impl(int bacterium_index) const = 0;

    virtual void eat_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void go_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void clon_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void str_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void left_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void right_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void back_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void turn_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void jg_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void jl_impl(
        const Params* params,
        int bacterium_index
    ) = 0;

    virtual void j_impl(
        const Abstract::Params* params,
        int bacterium_index
    ) = 0;

    virtual void je_impl(
        const Abstract::Params* params,
        int bacterium_index
    ) = 0;
};

}

namespace Implementation {

class LogicalChanger {
public:
    LogicalChanger(
        ModelPtr model,
        int team,
        int move_number
    );

    bool roundEnemySearch(
        int bacterium_index,
        Abstract::Point* enemy
    ) const;

    void eat(int bacterium_index);

    void go(int bacterium_index);

    void clon(int bacterium_index);

    void str(int bacterium_index);

    void left(int bacterium_index);

    void right(int bacterium_index);

    void back(int bacterium_index);

    void turn(int bacterium_index);

private:
    ModelPtr model_;
    int team_;
    int move_number_;

    void nextCoordinates(
        int direction,
        int steps,
        Abstract::Point& start,
        Abstract::Point* enemy = NULL
    ) const;

    void clonLogic(int bacterium_index);

    void strLogic(int bacterium_index);
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
        ModelPtr model,
        int team,
        int move_number,
        int instructions
    );

protected:
    void clearBeforeMove_impl();

    bool endOfMove_impl(int bacterium_index) const;

    int getBacteriaNumber_impl() const;

    int getTeam_impl() const;

    int getInstruction_impl(int bacterium_index) const;

    void eat_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void go_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void clon_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void str_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void left_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void right_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void back_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void turn_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void jg_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void jl_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void j_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

    void je_impl(
        const Abstract::Params* params,
        int bacterium_index
    );

private:
    ModelPtr model_;
    Ints remaining_actions_;
    Ints remaining_pseudo_actions_;
    // command = action OR pseudo action
    Ints completed_commands_;

    int team_;
    int move_number_;
    int instructions_;

    LogicalChanger logical_changer_;

    void markDead();

    void remainingActionsDecrement(
        Ints& remaining_commands_vect,
        int bacterium_index
    );

    void penalize(int bacterium_index);

    void updateInstruction(int bacterium_index);

    int checkCommandsNumber(int number) const;

    void repeater(RepeaterParams* params);
};

}

#endif
