#include <iostream>
#include <vector>
#include <string>

#include "filepath.hpp"
#include "asset_loader.hpp"
#include "model_loader.hpp"

class AssetManager {
private:
    std::vector<AssetLoader*> loaders;
    std::vector<Asset*> assets;

public:
    AssetManager() {
        loaders.push_back(new ModelLoader());
    }

    ~AssetManager() {
        for (auto loader : loaders) {
            delete loader;
        }
    }

    bool Load(const std::string& path) {
        std::string ext = GetFileExtension(path);
        for (auto loader : loaders) {
            if (loader->CanLoad(ext)) {
                return loader->Load(path);
            }
        }
        std::cout << "ERROR (AssetManager): Unsupported file extension: " << ext << std::endl;
        return false;
    }

private:
    
    

};

AssetManager assetManager = AssetManager();