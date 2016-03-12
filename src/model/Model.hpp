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

class Model {
public:
    enum CellState {
        EMPTY,
        BACTERIUM,
    };

    CellState cellState(int x, int y) const;

    int getTeam(int x, int y) const;

protected:
    Model(int size, int bacteria_number, int teams_number);

    virtual CellState cellState_impl(int x, int y) const = 0;

    virtual int getTeam_impl(int x, int y) const = 0;
};

}

namespace Implementation {

class Model : public Abstract::Model {
public:
    Model(int size, int bacteria_number, int teams_number);

protected:
    Abstract::Model::CellState cellState_impl(int x, int y) const;

    int getTeam_impl(int x, int y) const;

private:
    std::vector<int> board_;
};

}

#endif
