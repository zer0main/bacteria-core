/*
 * bacteria-core, core for cellular automaton
 * Copyright (C) 2016 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#ifndef YAML_READER_HPP_
#define YAML_READER_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "YamlConvert.hpp"
#include "CoreGlobals.hpp"
#include "Model.hpp"
#include "Exception.hpp"

class YamlReader {
public:
    YamlReader(const std::string& file_name);

    int getWidth() const;

    int getHeight() const;

    int getMovesNumber() const;

    Units getUnits() const;

private:
    std::string file_name_;
    YAML::Node yaml_node_;

    int getIntValue(const std::string& key) const;
};

#endif
