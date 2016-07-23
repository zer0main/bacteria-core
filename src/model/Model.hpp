/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <algorithm>
#include <vector>

#include "CoreGlobals.hpp"
#include "CoreConstants.hpp"
#include "Exception.hpp"

namespace Abstract {

template<typename TModel>
TModel* makeModel(int width, int height, int bacteria, int teams) {
    bool less = ((width < MIN_WIDTH) || (height < MIN_HEIGHT));
    bool greater = ((width > MAX_WIDTH) || (height > MAX_WIDTH));
    if (less || greater) {
        throw Exception("Model: width or height of board "
                        "is out of allowable range.");
    }
    if ((bacteria * teams) > ((width * height) / 2)) {
        throw Exception("Error: invalid number of creatures");
    }
    TModel* model = new TModel(width, height, bacteria, teams);
    return model;
}

enum CellState {
    EMPTY,
    BACTERIUM,
};

enum Direction {
    LEFT,
    FORWARD,
    RIGHT,
    BACKWARD,
};

struct Point {
    Point();

    Point(int x, int y);

    bool operator==(const Point& p) const;

    int x, y;
};

class Model {
public:
    void clearBeforeMove(int team);

    CellState cellState(const Point& coordinates) const;

    int getDirectionByCoordinates(
        const Point& coordinates
    ) const;

    int getMassByCoordinates(const Point& coordinates) const;

    int getTeamByCoordinates(const Point& coordinates) const;

    int getWidth() const;

    int getHeight() const;

    int getBacteriaNumber(int team) const;

    bool isAlive(int team, int bacterium_index) const;

    int getInstruction(int team, int bacterium_index) const;

    Point getCoordinates(
        int team,
        int bacterium_index
    ) const;

    int getDirection(int team, int bacterium_index) const;

    int getMass(int team, int bacterium_index) const;

    void kill(int team, int bacterium_index);

    void changeMass(int team, int bacterium_index, int change);

    void setDirection(
        int team,
        int bacterium_index,
        int new_direction
    );

    void setInstruction(
        int team,
        int bacterium_index,
        int new_instruction
    );

    void setCoordinates(
        int team,
        int bacterium_index,
        const Point& coordinates
    );

    void killByCoordinates(
        const Point& coordinates
    );

    void changeMassByCoordinates(
        const Point& coordinates,
        int change
    );

    void createNewByCoordinates(
        const Point& coordinates,
        int mass,
        int direction,
        int team,
        int instruction
    );

    virtual ~Model();

protected:
    Model(int width, int height, int bacteria, int teams);

    virtual void clearBeforeMove_impl(int team) = 0;

    virtual CellState cellState_impl(
        const Point& coordinates
    ) const = 0;

    virtual int getDirectionByCoordinates_impl(
        const Point& coordinates
    ) const = 0;

    virtual int getMassByCoordinates_impl(
        const Point& coordinates
    ) const = 0;

    virtual int getTeamByCoordinates_impl(
        const Point& coordinates
    ) const = 0;

    virtual int getWidth_impl() const = 0;

    virtual int getHeight_impl() const = 0;

    virtual int getBacteriaNumber_impl(int team) const = 0;

    virtual bool isAlive_impl(
        int team,
        int bacterium_index
    ) const = 0;

    virtual int getInstruction_impl(
        int team,
        int bacterium_index
    ) const = 0;

    virtual Point getCoordinates_impl(
        int team,
        int bacterium_index
    ) const = 0;

    virtual int getDirection_impl(
        int team,
        int bacterium_index
    ) const = 0;

    virtual int getMass_impl(
        int team,
        int bacterium_index
    ) const = 0;

    virtual void kill_impl(
        int team,
        int bacterium_index
    ) = 0;

    virtual void changeMass_impl(
        int team,
        int bacterium_index,
        int change
    ) = 0;

    virtual void setDirection_impl(
        int team,
        int bacterium_index,
        int new_direction
    ) = 0;

    virtual void setInstruction_impl(
        int team,
        int bacterium_index,
        int new_instruction
    ) = 0;

    virtual void setCoordinates_impl(
        int team,
        int bacterium_index,
        const Point& coordinates
    ) = 0;

    virtual void killByCoordinates_impl(
        const Point& coordinates
    ) = 0;

    virtual void changeMassByCoordinates_impl(
        const Point& coordinates,
        int change
    ) = 0;

    virtual void createNewByCoordinates_impl(
        const Point& coordinates,
        int mass,
        int direction,
        int team,
        int instruction
    ) = 0;
};

}

namespace Implementation {

struct Unit {
    Unit();

    Unit(
        const Abstract::Point& coordinates,
        int mass,
        int direction,
        int team,
        int instruction
    );

    Unit& operator=(const Unit& unit);

    Abstract::Point coordinates;
    int mass;
    int direction;
    int team;
    int instruction;
};

class Model : public Abstract::Model {
public:
    Model(int width, int height, int bacteria, int teams);

protected:
    void clearBeforeMove_impl(int team);

    Abstract::CellState cellState_impl(
        const Abstract::Point& coordinates
    ) const;

    int getDirectionByCoordinates_impl(
        const Abstract::Point& coordinates
    ) const;

    int getMassByCoordinates_impl(
        const Abstract::Point& coordinates
    ) const;

    int getTeamByCoordinates_impl(
        const Abstract::Point& coordinates
    ) const;

    int getWidth_impl() const;

    int getHeight_impl() const;

    int getBacteriaNumber_impl(int team) const;

    bool isAlive_impl(int team, int bacterium_index) const;

    int getInstruction_impl(int team, int bacterium_index) const;

    Abstract::Point getCoordinates_impl(
        int team,
        int bacterium_index
    ) const;

    int getDirection_impl(int team, int bacterium_index) const;

    int getMass_impl(int team, int bacterium_index) const;

    void kill_impl(int team, int bacterium_index);

    void changeMass_impl(int team, int bacterium_index, int change);

    void setDirection_impl(
        int team,
        int bacterium_index,
        int new_direction
    );

    void setInstruction_impl(
        int team,
        int bacterium_index,
        int new_instruction
    );

    void setCoordinates_impl(
        int team,
        int bacterium_index,
        const Abstract::Point& coordinates
    );

    void killByCoordinates_impl(
        const Abstract::Point& coordinates
    );

    void changeMassByCoordinates_impl(
        const Abstract::Point& coordinates,
        int change
    );

    void createNewByCoordinates_impl(
        const Abstract::Point& coordinates,
        int mass,
        int direction,
        int team,
        int instruction
    );

private:
    void initializeBoard(int bacteria, int teams);

    void tryToPlace(int team);

    void checkParams(
        int team_index,
        int bacterium_index,
        const char* method_name,
        bool check_alive
    ) const;

    void checkDead(
        int team,
        const char* method_name
    ) const;

    UnitPtrs board_;
    Teams teams_;

    // dead_bacteria_[team] is a number of dead bacteria for this team.
    // dead_bacteria_[team] is 0 after calling clearBeforeMove(team).
    Ints dead_bacteria_;

    int width_;
    int height_;
};

}

#endif
