/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef YAML_CONVERT_HPP_
#define YAML_CONVERT_HPP_

#include <yaml-cpp/yaml.h>

#include "Model.hpp"

namespace YAML {

template<>
struct convert<Implementation::Unit> {
    static Node encode(const Implementation::Unit& unit) {
        Node node;
        node["coordinates"][0] = unit.coordinates.x;
        node["coordinates"][1] = unit.coordinates.y;
        node["mass"] = unit.mass;
        node["direction"] = unit.direction;
        node["team"] = unit.team;
        node["instruction"] = unit.instruction;
        return node;
    }

    static bool decode(const Node& node, Implementation::Unit& unit) {
        // number of members of Unit
        int unit_members = 5;
        if (node.size() != unit_members) {
            return false;
        }
        Abstract::Point coordinates(
            node["coordinates"][0].as<int>(),
            node["coordinates"][1].as<int>()
        );
        unit.coordinates = coordinates;
        unit.mass = node["mass"].as<int>();
        unit.direction = node["direction"].as<int>();
        unit.team = node["team"].as<int>();
        unit.instruction = node["instruction"].as<int>();
        return true;
    }
};

}

#endif
