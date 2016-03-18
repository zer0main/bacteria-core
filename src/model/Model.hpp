/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <vector>

namespace Abstract {

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

class Model {
public:
    CellState cellState(int x, int y) const;

    int getDirection(int x, int y) const;

    int getMass(int x, int y) const;

    int getTeam(int x, int y) const;

    int getWidth() const;

    int getHeight() const;

protected:
    Model(int width, int height, int bacteria, int teams);

    virtual CellState cellState_impl(int x, int y) const = 0;

    virtual int getDirection_impl(int x, int y) const = 0;

    virtual int getMass_impl(int x, int y) const = 0;

    virtual int getTeam_impl(int x, int y) const = 0;

    virtual int getWidth_impl() const = 0;

    virtual int getHeight_impl() const = 0;
};

}

namespace Implementation {

struct Unit {
    Unit(int mass, int direction, int team, int instruction);

    int mass;
    int direction;
    int team;
    int instruction;
};

class Model : public Abstract::Model {
public:
    Model(int width, int height, int bacteria, int teams);

protected:
    Abstract::CellState cellState_impl(int x, int y) const;

    int getDirection_impl(int x, int y) const;

    int getMass_impl(int x, int y) const;

    int getTeam_impl(int x, int y) const;

    int getWidth_impl() const;

    int getHeight_impl() const;

private:
    void initializeBoard(int bacteria, int teams);

    std::vector<Unit*> board_;

    std::vector< std::vector<Unit> > units_;

    int width_;
    int height_;
};

}

#endif
