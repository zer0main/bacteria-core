/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_

namespace Abstract {

class Model {
public:
    enum CellState {
        BACTERIUM,
        EMPTY,
    };

    CellState cellState(int x, int y) const;

    int getTeam(int x, int y) const;

protected:
    Model(int size, int bacteria_number, int teams_number);

    virtual CellState cellState_impl(int x, int y) const = 0;

    virtual int getTeam_impl(int x, int y) const = 0;
};

}

#endif
