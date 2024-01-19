#pragma once

#include <string>

#include "ngine/object.hpp"


class Resource : public Object {
public:
    std::string name;

    Resource() = default;
    Resource(const Resource&) = default;
    Resource(const std::string& name)
        : name(name) {}
};