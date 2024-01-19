#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <map>

#include "core/filepath.hpp"
#include "ecs/asset_loader.hpp"
#include "renderer/model_loader.hpp"
#include "renderer/shader_loader.hpp"


class AssetManager {
private:
    std::vector<AssetLoader*> _loaders;
    std::vector<Asset*> _assets;

    std::map<std::string, uint32_t> _extToLoader;
    std::map<uint32_t, uint32_t> _idToType;

public:
    AssetManager() {}

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

    void AddLoader(AssetLoader* loader) {
        _loaders.push_back(loader);
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

    template<typename T>
    T& GetAsset() {

    }

};
