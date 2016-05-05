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

void Changer::clearBeforeMove() {
    return clearBeforeMove_impl();
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

void Changer::eat(const Params* params, int bacterium_index) {
    return eat_impl(params, bacterium_index);
}

void Changer::go(const Params* params, int bacterium_index) {
    return go_impl(params, bacterium_index);
}

void Changer::clon(const Params* params, int bacterium_index) {
    return clon_impl(params, bacterium_index);
}

void Changer::str(const Params* params, int bacterium_index) {
    return str_impl(params, bacterium_index);
}

void Changer::left(const Params* params, int bacterium_index) {
    return left_impl(params, bacterium_index);
}

void Changer::right(const Params* params, int bacterium_index) {
    return right_impl(params, bacterium_index);
}

void Changer::back(const Params* params, int bacterium_index) {
    return back_impl(params, bacterium_index);
}

void Changer::turn(const Params* params, int bacterium_index) {
    return turn_impl(params, bacterium_index);
}

void Changer::jg(const Params* params, int bacterium_index) {
    return jg_impl(params, bacterium_index);
}

void Changer::jl(const Params* params, int bacterium_index) {
    return jl_impl(params, bacterium_index);
}

void Changer::j(const Params* params, int bacterium_index) {
    return j_impl(params, bacterium_index);
}

void Changer::je(const Params* params, int bacterium_index) {
    return je_impl(params, bacterium_index);
}

Changer::Changer(
    ModelPtr /*model*/,
    int /*team*/,
    int /*move_number*/,
    int /*instructions*/
) {
}

}

namespace Implementation {

static void eraseElements(Ints& vect, int val) {
    Ints::iterator begin = vect.begin();
    Ints::iterator end = vect.end();
    vect.erase(std::remove(begin, end, val), end);
}

LogicalChanger::LogicalChanger(
    ModelPtr model,
    int team,
    int move_number
)
    : model_(model)
    , team_(team)
    , move_number_(move_number)
{
}

void LogicalChanger::eat(int bacterium_index) {
    model_->changeMass(team_, bacterium_index, EAT_MASS);
}

void LogicalChanger::go(int bacterium_index) {
    model_->changeMass(team_, bacterium_index, GO_MASS);
    int mass = model_->getMass(team_, bacterium_index);
    if (mass <= 0) {
        model_->kill(team_, bacterium_index);
    } else {
        Abstract::Point coordinates = model_->getCoordinates(
            team_,
            bacterium_index
        );
        int direction = model_->getDirection(team_, bacterium_index);
        nextCoordinates(direction, team_, 1, coordinates);
        Abstract::CellState state = model_->cellState(coordinates);
        if (state == Abstract::EMPTY) {
            model_->setCoordinates(team_, bacterium_index, coordinates);
        }
    }
}

void LogicalChanger::clon(int bacterium_index) {
    model_->changeMass(team_, bacterium_index, CLON_MASS);
    int mass = model_->getMass(team_, bacterium_index);
    if (mass < 0) {
        model_->kill(team_, bacterium_index);
    } else if (mass == 0) {
        clonLogic(bacterium_index);
        model_->kill(team_, bacterium_index);
    } else {
        clonLogic(bacterium_index);
    }
}

void LogicalChanger::str(int bacterium_index) {
    model_->changeMass(team_, bacterium_index, STR_MASS);
    int mass = model_->getMass(team_, bacterium_index);
    if (mass < 0) {
        model_->kill(team_, bacterium_index);
    } else if (mass == 0) {
        strLogic(bacterium_index);
        model_->kill(team_, bacterium_index);
    } else {
        strLogic(bacterium_index);
    }
}

void LogicalChanger::left(int bacterium_index) {
    int direction = model_->getDirection(team_, bacterium_index);
    direction = ((direction == 0) ? 3 : (direction - 1));
    model_->setDirection(team_, bacterium_index, direction);
}

void LogicalChanger::right(int bacterium_index) {
    int direction = model_->getDirection(team_, bacterium_index);
    direction = ((direction == 3) ? 0 : (direction + 1));
    model_->setDirection(team_, bacterium_index, direction);
}

void LogicalChanger::back(int bacterium_index) {
    int direction = model_->getDirection(team_, bacterium_index);
    direction = (direction + 2) % 4;
    model_->setDirection(team_, bacterium_index, direction);
}

void LogicalChanger::turn(int bacterium_index) {
    int direction = random(4);
    model_->setDirection(team_, bacterium_index, direction);
}

void LogicalChanger::clonLogic(int bacterium_index) {
    int direction = model_->getDirection(team_, bacterium_index);
    Abstract::Point coordinates = model_->getCoordinates(
        team_,
        bacterium_index
    );
    Abstract::Point temp = coordinates;
    nextCoordinates(direction, team_, 1, coordinates);
    Abstract::CellState state = model_->cellState(coordinates);
    bool equal = ((temp.x == coordinates.x) &&
                  (temp.y == coordinates.y));
    if (state == Abstract::EMPTY) {
        model_->createNewByCoordinates(
            coordinates,
            DEFAULT_CLON_MASS,
            random(4),
            team_,
            0
        );
    } else if (!equal) {
        model_->changeMassByCoordinates(coordinates, DEFAULT_CLON_MASS);
    }
}

void LogicalChanger::strLogic(int bacterium_index) {
    int mass = model_->getMass(team_, bacterium_index);
    int damage = random(-MAX_STR_DAMAGE) + mass / 2;
}

void LogicalChanger::nextCoordinates(
    int direction,
    int team,
    int steps,
    Abstract::Point& start,
    Abstract::Point* enemy
) const {
    int max_width = model_->getWidth() - 1;
    int max_height = model_->getHeight() - 1;
    enemy = NULL;
    for (int i = 0; i < steps; i++) {
        if ((direction == Abstract::LEFT) &&
            (start.x > 0)) {
            start.x--;
        } else if ((direction == Abstract::RIGHT) &&
                   (start.x < max_width)) {
            start.x++;
        } else if ((direction == Abstract::BACKWARD) &&
                   (start.y > 0)) {
            start.y--;
        } else if ((direction == Abstract::FORWARD) &&
                   (start.y < max_height)) {
            start.y++;
        }
        Abstract::CellState state = model_->cellState(start);
        if ((enemy == NULL) && (state == Abstract::BACTERIUM)) {
            int team2 = model_->getTeamByCoordinates(start);
            if (team != team2) {
                *enemy = start;
            }
        }
    }
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
    ModelPtr model,
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
    int bacteria = model_->getBacteriaNumber(team_);
    remaining_actions_.resize(bacteria, MAX_ACTIONS);
    remaining_pseudo_actions_.resize(bacteria, MAX_PSEUDO_ACTIONS);
    completed_commands_.resize(bacteria, 0);
}

void Changer::clearBeforeMove_impl() {
    markDead();
    // remove dead
    eraseElements(remaining_actions_, -1);
    eraseElements(remaining_pseudo_actions_, -1);
    eraseElements(completed_commands_, -1);
    // clear model
    model_->clearBeforeMove(team_);
    // resize Changer's private vectors
    int bacteria = getBacteriaNumber_impl();
    remaining_actions_.resize(bacteria, MAX_ACTIONS);
    remaining_pseudo_actions_.resize(bacteria, MAX_PSEUDO_ACTIONS);
    completed_commands_.resize(bacteria, 0);
    // max remaining actions (because of new move)
    for (int b = 0; b < bacteria; b++) {
        remaining_actions_[b] = MAX_ACTIONS;
        remaining_pseudo_actions_[b] = MAX_PSEUDO_ACTIONS;
    }
}

bool Changer::endOfMove_impl(int bacterium_index) const {
    bool actions = remaining_actions_[bacterium_index] > 0;
    bool pseudo_actions = remaining_pseudo_actions_[bacterium_index] > 0;
    bool alive = model_->isAlive(team_, bacterium_index);
    return !(actions && pseudo_actions && alive);
}

int Changer::getBacteriaNumber_impl() const {
    return model_->getBacteriaNumber(team_);
}

int Changer::getTeam_impl() const {
    return team_;
}

int Changer::getInstruction_impl(int bacterium_index) const {
    return model_->getInstruction(team_, bacterium_index);
}

void Changer::eat_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    if (params->spec) {
        n = random(RANDOM_MAX_ACTIONS);
    } else if (params->p1 != -1) {
        n = checkCommandsNumber(params->p1);
    }
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_actions_,
        &LogicalChanger::eat
    );
    repeater(&rp);
}

void Changer::go_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    if (params->spec) {
        n = random(RANDOM_MAX_ACTIONS);
    } else if (params->p1 != -1) {
        n = checkCommandsNumber(params->p1);
    }
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_actions_,
        &LogicalChanger::go
    );
    repeater(&rp);
}

void Changer::clon_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_actions_,
        &LogicalChanger::clon
    );
    repeater(&rp);
}

void Changer::str_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    if (params->p1 != -1) {
        n = checkCommandsNumber(params->p1);
    }
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_actions_,
        &LogicalChanger::str
    );
    repeater(&rp);
}

void Changer::left_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    if (params->p1 != -1) {
        n = checkCommandsNumber(params->p1);
    }
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_pseudo_actions_,
        &LogicalChanger::left
    );
    repeater(&rp);
    penalize(bacterium_index);
}

void Changer::right_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    if (params->p1 != -1) {
        n = checkCommandsNumber(params->p1);
    }
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_pseudo_actions_,
        &LogicalChanger::right
    );
    repeater(&rp);
    penalize(bacterium_index);
}

void Changer::back_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_pseudo_actions_,
        &LogicalChanger::back
    );
    repeater(&rp);
    penalize(bacterium_index);
}

void Changer::turn_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int n = 1;
    RepeaterParams rp(
        bacterium_index,
        n,
        remaining_pseudo_actions_,
        &LogicalChanger::turn
    );
    repeater(&rp);
    penalize(bacterium_index);
}

void Changer::jg_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int mass = model_->getMass(team_, bacterium_index);
    if (mass > params->p1) {
        int instruction = params->p2;
        if ((instruction >= 0) && instruction < instructions_) {
            model_->setInstruction(team_, bacterium_index, instruction);
        } else {
            throw Exception("Invalid instruction in jg command.");
        }
    } else {
        updateInstruction(bacterium_index);
    }
    remainingActionsDecrement(
        remaining_pseudo_actions_,
        bacterium_index
    );
    penalize(bacterium_index);
}

void Changer::jl_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int mass = model_->getMass(team_, bacterium_index);
    if (mass < params->p1) {
        int instruction = params->p2;
        if ((instruction >= 0) && instruction < instructions_) {
            model_->setInstruction(team_, bacterium_index, instruction);
        } else {
            throw Exception("Invalid instruction in jl command.");
        }
    } else {
        updateInstruction(bacterium_index);
    }
    remainingActionsDecrement(
        remaining_pseudo_actions_,
        bacterium_index
    );
    penalize(bacterium_index);
}

void Changer::j_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
    int instruction = params->p1;
    if ((instruction >= 0) && (instruction < instructions_)) {
        model_->setInstruction(team_, bacterium_index, instruction);
    } else {
        throw Exception("Invalid instruction in j command.");
    }
    remainingActionsDecrement(
        remaining_pseudo_actions_,
        bacterium_index
    );
    penalize(bacterium_index);
}

void Changer::je_impl(
    const Abstract::Params* params,
    int bacterium_index
) {
}

void Changer::markDead() {
    int bacteria = remaining_actions_.size();
    for (int b = 0; b < bacteria; b++) {
        if (!model_->isAlive(team_, b)) {
            remaining_actions_[b] = -1;
            remaining_pseudo_actions_[b] = -1;
            completed_commands_[b] = -1;
        }
    }
}

void Changer::remainingActionsDecrement(
    Ints& actions_vect,
    int bacterium_index
) {
    bool less = bacterium_index < 0;
    bool greater = bacterium_index >= actions_vect.size();
    if (less || greater) {
        throw Exception("Changer: invalid bacterium index.");
    }
    actions_vect[bacterium_index]--;
    if (actions_vect[bacterium_index] < 0) {
        throw Exception("Changer: too many commands for one move.");
    }
}

void Changer::penalize(int bacterium_index) {
    bool alive = model_->isAlive(team_, bacterium_index);
    bool end = endOfMove(bacterium_index);
    if (alive && end) {
        model_->changeMass(
            team_,
            bacterium_index,
            PSEUDO_ACTIONS_EXCESS_PENALTY
        );
        int mass = model_->getMass(team_, bacterium_index);
        if (mass <= 0) {
            model_->kill(team_, bacterium_index);
        }
    }
}

void Changer::updateInstruction(int index) {
    completed_commands_[index] = 0;
    int instruction = model_->getInstruction(team_, index);
    if ((instruction + 1) < instructions_) {
        model_->setInstruction(team_, index, instruction + 1);
    } else {
        model_->setInstruction(team_, index, 0);
    }
}

int Changer::checkCommandsNumber(int number) const {
    bool greater = number > MIN_COMMANDS_PER_INSTRUCTION;
    bool less = number < MAX_COMMANDS_PER_INSTRUCTION;
    if (greater && less) {
        return number;
    } else {
        throw Exception("Changer: invalid commands number.");
    }
}

void Changer::repeater(RepeaterParams* params) {
    int index = params->bacterium_index;
    int total_commands = params->commands;
    while (!endOfMove(index) &&
           (completed_commands_[index] < total_commands)) {
        remainingActionsDecrement(
            params->remaining_commands_vect,
            index
        );
        completed_commands_[index]++;
        LogicalMethod method = params->logic_function;
        (logical_changer_.*method)(index);
    }
    if (model_->isAlive(team_, index)) {
        if (completed_commands_[index] == (total_commands)) {
            updateInstruction(index);
        }
    }
}

}
