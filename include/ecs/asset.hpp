#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "ngine/object.hpp"


class Asset : public Object {
    public:
        std::string name;
        std::string path;

        Asset() = default;
        Asset(const Asset&) = default;
        Asset(const std::string& name, const std::string& path)
            : name(name), path(path) {}

};
