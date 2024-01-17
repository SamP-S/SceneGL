#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "ecs/filepath.hpp"
#include "ecs/asset.hpp"

class AssetLoader {
private:
    std::vector<std::string> _extensions;
    
public:
    virtual bool Load(const std::string& filepath) = 0;
    virtual std::vector<std::string> GetSupportedExt() = 0;
    virtual bool CanLoad(const std::string& filepath) = 0;
    
};