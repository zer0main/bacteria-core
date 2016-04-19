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

bool Changer::endOfMove(int bacterium_index) const {
    return endOfMove_impl(bacterium_index);
}

int Changer::getBacteriaNumber() const {
    return getBacteriaNumber_impl();
}

int Changer::getTeam() const {
    return getTeam_impl();
}

int Changer::getInstruction(int bacterium_index) const {
    return getInstruction_impl(bacterium_index);
}

Changer::Changer(
    Model& /*model*/,
    int /*team*/,
    int /*move_number*/,
    int /*instructions*/
) {
}

}

namespace Implementation {

LogicalChanger::LogicalChanger(
    Abstract::Model& model,
    int team,
    int move_number
)
    : model_(model)
    , team_(team)
    , move_number_(move_number)
{
}

void LogicalChanger::eat(int bacterium_index) {
    model_.changeMass(team_, bacterium_index, EAT_MASS);
}

RepeaterParams::RepeaterParams(
    int bacterium_index,
    int commands,
    Ints& remaining_commands_vect,
    LogicalMethod logic_function
)
    : bacterium_index(bacterium_index)
    , commands(commands)
    , remaining_commands_vect(remaining_commands_vect)
    , logic_function(logic_function)
{
}

Changer::Changer(
    Abstract::Model& model,
    int team,
    int move_number,
    int instructions
)
    : Abstract::Changer(model, team, move_number, instructions)
    , model_(model)
    , team_(team)
    , move_number_(move_number)
    , instructions_(instructions)
    , logical_changer_(model_, team_, move_number_) {
    int bacteria = model_.getBacteriaNumber(team_);
    remaining_actions_.resize(bacteria, MAX_ACTIONS);
    remaining_pseudo_actions_.resize(bacteria, MAX_PSEUDO_ACTIONS);
    completed_commands_.resize(bacteria, 0);
}

bool Changer::endOfMove_impl(int bacterium_index) const {
    bool actions = remaining_actions_[bacterium_index] > 0;
    bool pseudo_actions = remaining_pseudo_actions_[bacterium_index] > 0;
    return !(actions && pseudo_actions);
}

int Changer::getBacteriaNumber_impl() const {
    return model_.getBacteriaNumber(team_);
}

int Changer::getTeam_impl() const {
    return team_;
}

int Changer::getInstruction_impl(int bacterium_index) const {
    return model_.getInstruction(team_, bacterium_index);
}

}

}
