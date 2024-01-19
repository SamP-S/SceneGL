#pragma once

// std libs
#include <memory>
// internal libs
#include "core/ngine.hpp"
// external libs
#include "json.hpp"

class ISerializer {
    public:
        ISerializer(std::shared_ptr<Scene> scene) 
            : _scene(scene) {}
        
        virtual void Serialize(const std::string& filepath) = 0;
        virtual void Deserialize(const std::string& filepath) = 0;

    private:
        std::shared_ptr<Scene> _scene;
};

class JsonSerializer : public ISerializer {
    public:
        JsonSerializer(std::shared_ptr<Scene> scene)
            : ISerializer(scene) {}

        // read in json file to scene
        void Serialize(const std::string& filepath) override {
            // not implemented
        }

        // output json file to scene
        void Deserialize(const std::string& filepath) override {
            // not implemented
            
        }
};