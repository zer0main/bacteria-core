/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "YamlReader.hpp"

YamlReader::YamlReader(
    const std::string& file_name
)
    : file_name_(file_name)
    , yaml_node_(YAML::LoadFile(file_name))
{
}

int YamlReader::getWidth() const {
    return getIntValue("board_width");
}

int YamlReader::getHeight() const {
    return getIntValue("board_height");
}

int YamlReader::getMovesNumber() const {
    return getIntValue("moves_number");
}

Units YamlReader::getUnits() const {
    Units units;
    if (yaml_node_["units"]) {
        for (int i = 0; i < yaml_node_["units"].size(); i++) {
            units.push_back(
                yaml_node_["units"][i].as<Implementation::Unit>()
            );
        }
    }
    return units;
}

int YamlReader::getIntValue(const std::string& key) const {
    if (yaml_node_[key]) {
        return yaml_node_[key].as<int>();
    } else {
        throw Exception(
            "YamlReader: " + key + " is not specified "
            "in " + file_name_
        );
    }
}
