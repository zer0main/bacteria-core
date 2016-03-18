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
    int getTeam(int x, int y) const;

    int getWidth() const;

    int getHeight() const;

protected:
    Model(int width, int height);

    virtual int getTeam_impl(int x, int y) const = 0;

    virtual int getWidth_impl() const = 0;

    virtual int getHeight_impl() const = 0;
};

}

namespace Implementation {

struct Cell {
    int mass;
    int direction;
    int team;
    int instruction;
};

class Model : public Abstract::Model {
public:
    Model(int width, int height);

protected:
    int getTeam_impl(int x, int y) const;

    int getWidth_impl() const;

    int getHeight_impl() const;

private:
    std::vector<Cell> board_;

    int width_;
    int height_;
};

}

#endif
