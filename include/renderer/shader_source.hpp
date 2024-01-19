#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "tai/tai.hpp"

enum ShaderStage {
    INVALID = 0,
    VERTEX = 1,
    FRAGMENT = 2,
    COMPUTE = 3,
    GEOMETRY = 4,
    TESSELLATION_CONTROL = 5,
    TESSELLATION_EVALUATION = 6
};

class ShaderSource : public Tai::Asset {
    public:
        std::string source = "";
        ShaderStage stage = ShaderStage::INVALID;
    
        ShaderSource(std::string name="Default Shader Stage", const std::string& source="", ShaderStage stage=ShaderStage::INVALID)
            : Asset(name), source(source), stage(stage) {}

        virtual uint32_t Compile() = 0;

};
