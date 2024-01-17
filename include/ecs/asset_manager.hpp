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
    std::vector<AssetLoader*> _loaders;
    std::vector<Asset*> _assets;

public:
    AssetManager() {
        _loaders.push_back(new ModelLoader());
        _loaders.push_back(new ShaderLoader());
    }

    ~AssetManager() {
        // clear loaders
        for (AssetLoader* loader : _loaders) {
            delete loader;
        }
        _loaders.clear();
        // clear assets
        for (Asset* asset : _assets) {
            delete asset;
        }
        _assets.clear();
    }

    bool Load(const std::string& filepath) {
        for (auto loader : _loaders) {
            if (loader != nullptr && loader->CanLoad(filepath)) {
                return loader->Load(filepath);
            }
        }
        std::cout << "ERROR (AssetManager): Unsupported file extension @ " << filepath << std::endl;
        return false;
    }

};

AssetManager assetManager = AssetManager();