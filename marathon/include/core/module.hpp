#pragma once

#include <string>
#include <map>

//// TODO:
// Should probably keep modules in a registry together

// as enum for implicit int conversion
enum class ModuleType {
    WINDOW,
    INPUT,
    MATHS,
    RENDER,
    AUDIO,
    MAX_ENUM
};

class Module {
public:
    // standard methods for safe start/end/rebooting modules
    virtual void Boot() = 0;
    virtual void Shutdown() = 0;
    
    virtual std::string GetName() = 0;
    virtual ModuleType GetType() = 0;
};
