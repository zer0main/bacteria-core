/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "Model.hpp"
#include "random.hpp"

namespace Abstract {

Model::Model(
    int /*width*/,
    int /*height*/,
    int /*bacteria*/,
    int /*teams*/
) {
}

Point::Point() {
}

Point::Point(
    int x,
    int y
)
    : x(x)
    , y(y) {
}

bool Point::operator==(const Point& p) const {
    return (p.x == x) && (p.y == y);
}

void Model::clearBeforeMove(int team) {
    return clearBeforeMove_impl(team);
}

CellState Model::cellState(const Point& coordinates) const {
    return cellState_impl(coordinates);
}

int Model::getDirectionByCoordinates(
    const Point& coordinates
) const {
    return getDirectionByCoordinates_impl(coordinates);
}

int Model::getMassByCoordinates(
    const Point& coordinates
) const {
    return getMassByCoordinates_impl(coordinates);
}

int Model::getTeamByCoordinates(
    const Point& coordinates
) const {
    return getTeamByCoordinates_impl(coordinates);
}

int Model::getWidth() const {
    return getWidth_impl();
}

int Model::getHeight() const {
    return getHeight_impl();
}

int Model::getBacteriaNumber(int team) const {
    return getBacteriaNumber_impl(team);
}

bool Model::isAlive(int team, int bacterium_index) const {
    return isAlive_impl(team, bacterium_index);
}

int Model::getInstruction(int team, int bacterium_index) const {
    return getInstruction_impl(team, bacterium_index);
}

Point Model::getCoordinates(
    int team,
    int bacterium_index
) const {
    return getCoordinates_impl(team, bacterium_index);
}

int Model::getDirection(int team, int bacterium_index) const {
    return getDirection_impl(team, bacterium_index);
}

int Model::getMass(int team, int bacterium_index) const {
    return getMass_impl(team, bacterium_index);
}

void Model::kill(int team, int bacterium_index) {
    return kill_impl(team, bacterium_index);
}

void Model::changeMass(int team, int bacterium_index, int change) {
    return changeMass_impl(team, bacterium_index, change);
}

void Model::setDirection(
    int team,
    int bacterium_index,
    int new_direction
) {
    return setDirection_impl(
        team,
        bacterium_index,
        new_direction
    );
}

void Model::setInstruction(
    int team,
    int bacterium_index,
    int new_instruction
) {
    return setInstruction_impl(
        team,
        bacterium_index,
        new_instruction
    );
}

void Model::setCoordinates(
    int team,
    int bacterium_index,
    const Point& coordinates
) {
    return setCoordinates_impl(
        team,
        bacterium_index,
        coordinates
    );
}

void Model::killByCoordinates(
    const Point& coordinates
) {
    return killByCoordinates_impl(coordinates);
}

void Model::changeMassByCoordinates(
    const Point& coordinates,
    int change
) {
    return changeMassByCoordinates_impl(coordinates, change);
}

void Model::createNewByCoordinates(
    const Point& coordinates,
    int mass,
    int direction,
    int team,
    int instruction
) {
    return createNewByCoordinates_impl(
        coordinates,
        mass,
        direction,
        team,
        instruction
    );
}

Model::~Model() {
}

}

namespace Implementation {

static bool isNull(UnitPtr unit_ptr) {
    return unit_ptr.isNull();
}

static bool checkIndex(int index, int size) {
    return (index >= 0) && (index < size);
}

/* get global coordinate from horizontal and
   vertical coordinates
*/
static int getIndex(
    const Abstract::Point& coordinates,
    int width,
    int height
) {
    bool less = ((coordinates.x < 0) || (coordinates.y < 0));
    bool greater = ((coordinates.x >= width) ||
                    (coordinates.y >= height));
    if (less || greater) {
        throw Exception(
            "Model: index of cell in arguments "
            "of some methods is out of range."
        );
    }
    int index = coordinates.y * width + coordinates.x;
    return index;
}

Unit::Unit() {
}

Unit::Unit(
    const Abstract::Point& coordinates,
    int mass,
    int direction,
    int team,
    int instruction
)
    : coordinates(coordinates)
    , mass(mass)
    , direction(direction)
    , team(team)
    , instruction(instruction) {
}

Unit& Unit::operator=(const Unit& unit) {
    coordinates = unit.coordinates;
    mass = unit.mass;
    direction = unit.direction;
    team = unit.team;
    instruction = unit.instruction;
    return *this;
}

Model::Model(
    int width,
    int height,
    int bacteria,
    int teams
)
    : Abstract::Model(width, height, bacteria, teams)
    , width_(width)
    , height_(height) {
    board_.resize(width * height);
    teams_.resize(teams);
    dead_bacteria_.resize(teams, 0);
    initializeBoard(bacteria, teams);
}

void Model::clearBeforeMove_impl(int team) {
    if (!checkIndex(team, teams_.size())) {
        throw Exception(
            "Model: team argument of "
            "clearBeforeMove() method is out of "
            "allowable range."
        );
    }
    UnitPtrs::iterator begin = teams_[team].begin();
    UnitPtrs::iterator end = teams_[team].end();
    teams_[team].erase(std::remove_if(begin, end, isNull), end);
    dead_bacteria_[team] = 0;
}

Abstract::CellState Model::cellState_impl(
    const Abstract::Point& coordinates
) const {
    int index = getIndex(coordinates, width_, height_);
    UnitPtr unit_ptr = board_[index];
    if (!unit_ptr.isNull()) {
        return Abstract::BACTERIUM;
    } else {
        return Abstract::EMPTY;
    }
}

int Model::getDirectionByCoordinates_impl(
    const Abstract::Point& coordinates
) const {
    int index = getIndex(coordinates, width_, height_);
    UnitPtr unit_ptr = board_[index];
    if (!unit_ptr.isNull()) {
        return unit_ptr->direction;
    } else {
        throw Exception("Error: Attempt to get direction of empty cell.");
    }
}

int Model::getMassByCoordinates_impl(
    const Abstract::Point& coordinates
) const {
    int index = getIndex(coordinates, width_, height_);
    UnitPtr unit_ptr = board_[index];
    if (!unit_ptr.isNull()) {
        return unit_ptr->mass;
    } else {
        throw Exception("Error: Attempt to get mass of empty cell.");
    }
}

int Model::getTeamByCoordinates_impl(
    const Abstract::Point& coordinates
) const {
    int index = getIndex(coordinates, width_, height_);
    UnitPtr unit_ptr = board_[index];
    if (!unit_ptr.isNull()) {
        return unit_ptr->team;
    } else {
        // no unit in the current cell
        throw Exception("Error: Attempt to get team of empty cell.");
    }
}

int Model::getWidth_impl() const {
    return width_;
}

int Model::getHeight_impl() const {
    return height_;
}

int Model::getBacteriaNumber_impl(int team) const {
    if (!checkIndex(team, teams_.size())) {
        throw Exception(
            "Model: team argument of "
            "getBacteriaNumber() method is out of "
            "allowable range."
        );
    }
    checkDead(team, "getBacteriaNumber()");
    return teams_[team].size();
}

bool Model::isAlive_impl(
    int team,
    int bacterium_index
) const {
    checkParams(team, bacterium_index, "isAlive()", false);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    return !unit_ptr.isNull();
}

int Model::getInstruction_impl(
    int team,
    int bacterium_index
) const {
    checkParams(team, bacterium_index, "getInstruction()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    return unit_ptr->instruction;
}

Abstract::Point Model::getCoordinates_impl(
    int team,
    int bacterium_index
) const {
    checkParams(team, bacterium_index, "getCoordinates()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    Abstract::Point coordinates = unit_ptr->coordinates;
    return coordinates;
}

int Model::getDirection_impl(int team, int bacterium_index) const {
    checkParams(team, bacterium_index, "getDirection()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    return unit_ptr->direction;
}

int Model::getMass_impl(int team, int bacterium_index) const {
    checkParams(team, bacterium_index, "getMass()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    return unit_ptr->mass;
}

void Model::kill_impl(
    int team,
    int bacterium_index
) {
    checkParams(team, bacterium_index, "kill()", true);
    Abstract::Point coordinates =
        teams_[team][bacterium_index]->coordinates;
    teams_[team][bacterium_index] = UnitPtr(0);
    dead_bacteria_[team]++;
    int index = getIndex(coordinates, width_, height_);
    board_[index] = UnitPtr(0);
}

void Model::changeMass_impl(
    int team,
    int bacterium_index,
    int change
) {
    checkParams(team, bacterium_index, "changeMass()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    unit_ptr->mass += change;
}

void Model::setDirection_impl(
    int team,
    int bacterium_index,
    int new_direction
) {
    checkParams(team, bacterium_index, "setDirection()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    unit_ptr->direction = new_direction;
}

void Model::setInstruction_impl(
    int team,
    int bacterium_index,
    int new_instruction
) {
    checkParams(team, bacterium_index, "setInstruction()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    unit_ptr->instruction = new_instruction;
}

void Model::setCoordinates_impl(
    int team,
    int bacterium_index,
    const Abstract::Point& coordinates
) {
    checkParams(team, bacterium_index, "setCoordinates()", true);
    UnitPtr unit_ptr = teams_[team][bacterium_index];
    Abstract::Point prev_coordinates = unit_ptr->coordinates;
    int prev_index = getIndex(prev_coordinates, width_, height_);
    int new_index = getIndex(coordinates, width_, height_);
    board_[prev_index] = UnitPtr(0);
    board_[new_index] = unit_ptr;
    unit_ptr->coordinates = coordinates;
}

void Model::killByCoordinates_impl(
    const Abstract::Point& coordinates
) {
    int index = getIndex(coordinates, width_, height_);
    UnitPtr murdered = board_[index];
    if (murdered.isNull()) {
        throw Exception(
            "Model: Attempt to call killByCoordinates() "
            "with coordinates of empty cell."
        );
    }
    int team = murdered->team;
    board_[index] = UnitPtr(0);
    UnitPtrs::iterator for_kill = std::find(
        teams_[team].begin(),
        teams_[team].end(),
        murdered
    );
    *for_kill = UnitPtr(0);
    dead_bacteria_[team]++;
}

void Model::changeMassByCoordinates_impl(
    const Abstract::Point& coordinates,
    int change
) {
    int index = getIndex(coordinates, width_, height_);
    UnitPtr unit_ptr = board_[index];
    if (!unit_ptr.isNull()) {
        unit_ptr->mass += change;
    } else {
        throw Exception("Error: Attempt to change mass of empty cell.");
    }
}

void Model::createNewByCoordinates_impl(
    const Abstract::Point& coordinates,
    int mass,
    int direction,
    int team,
    int instruction
) {
    UnitPtr unit_ptr(new Unit(
        coordinates,
        mass,
        direction,
        team,
        instruction
    ));
    teams_[team].push_back(unit_ptr);
    int index = getIndex(coordinates, width_, height_);
    board_[index] = unit_ptr;
}

void Model::initializeBoard(int bacteria, int teams) {
    for (int team = 0; team < teams; team++) {
        for (int bacterium = 0; bacterium < bacteria; bacterium++) {
            tryToPlace(team);
        }
    }
}

void Model::tryToPlace(int team) {
    int x = random(width_);
    int y = random(height_);
    while (cellState(Abstract::Point(x, y)) != Abstract::EMPTY) {
        x = random(width_);
        y = random(height_);
    }
    int direction = random(4);
    UnitPtr unit_ptr(new Unit(
        Abstract::Point(x, y),
        DEFAULT_MASS,
        direction,
        team,
        0
    ));
    teams_[team].push_back(unit_ptr);
    int index = getIndex(Abstract::Point(x, y), width_, height_);
    board_[index] = unit_ptr;
}

#define TO_S std::string
void Model::checkParams(
    int team,
    int bacterium_index,
    const char* method_name,
    bool check_alive
) const {
    if (!checkIndex(team, teams_.size())) {
        throw Exception(
            "Model: team argument of " + TO_S(method_name) +
            " is out of allowable range."
        );
    }
    if (!checkIndex(bacterium_index, teams_[team].size())) {
        throw Exception(
            "Model: bacterium_index argument of " +
            TO_S(method_name) + " is out of allowable range"
        );
    }
    if (check_alive) {
        UnitPtr unit_ptr = teams_[team][bacterium_index];
        if (unit_ptr.isNull()) {
            throw Exception(
                "Model: Attempt to call " + TO_S(method_name) +
                " with NULL ptr."
            );
        }
    }
}

void Model::checkDead(
    int team,
    const char* method_name
) const {
    if (dead_bacteria_[team] > 0) {
        throw Exception(
            "Model: there are some dead bacteria; you "
            "must call clearBeforeMove() before " + TO_S(method_name)
        );
    }
}
#undef TO_S

}
