/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef YAML_EMITTER_HPP_
#define YAML_EMITTER_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "YamlConvert.hpp"
#include "CoreGlobals.hpp"
#include "Model.hpp"

class YamlEmitter {
public:
    void writeWidth(int width);

    void writeHeight(int height);

    void writeUnits(const Units& units);

    YAML::Node out_;
};

#endif
