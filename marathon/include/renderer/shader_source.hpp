#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ngine/ngine.hpp"

enum class ShaderStage {
    INVALID = 0,
    VERTEX = 1,
    FRAGMENT = 2,
    COMPUTE = 3,
    GEOMETRY = 4,
    TESSELLATION_CONTROL = 5,
    TESSELLATION_EVALUATION = 6
};

class ShaderSource : public Asset {
    public:
        std::string source = "";
        ShaderStage stage = ShaderStage::INVALID;
    
        ShaderSource(const std::string& name="Default Shader Stage", const std::string& source="", const ShaderStage& stage=ShaderStage::INVALID)
            : Asset(name), source(source), stage(stage) {}

        virtual uint32_t Compile() = 0;

};
