#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

#include "ecs/filepath.hpp"
#include "ecs/asset_loader.hpp"
#include "renderer/model_loader.hpp"
#include "renderer/shader_loader.hpp"

class AssetManager {
private:
    std::vector<AssetLoader*> loaders;
    std::vector<Asset*> assets;

public:
    AssetManager() {
        loaders.push_back(new ModelLoader());
        loaders.push_back(new ShaderLoader());
    }

    ~AssetManager() {
        for (auto loader : loaders) {
            delete loader;
        }
    }

    bool Load(const std::string& path) {
        std::string ext = GetFileExtension(path);
        for (auto loader : loaders) {
            ModelLoader* modelLoader = dynamic_cast<ModelLoader*>(loader);
            if (modelLoader != nullptr && modelLoader->CanLoad(ext)) {
                return modelLoader->Load(path);
            }
        }
        std::cout << "ERROR (AssetManager): Unsupported file extension: " << ext << std::endl;
        return false;
    }

};

AssetManager assetManager = AssetManager();