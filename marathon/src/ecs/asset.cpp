#include "ecs/asset.hpp"

Asset::Asset(const std::string& name, const std::string& path)
    : name(name), path(path) {}


AssetManager& AssetManager::Instance() {
    static AssetManager instance;
    return instance;
}

AssetLoaderManager& AssetLoaderManager::Instance() {
    static AssetLoaderManager instance;
    return instance;
}

void AssetLoaderManager::AddLoader(std::shared_ptr<IAssetLoader> loader) {
    _loaders.push_back(loader);
}

bool AssetLoaderManager::Load(const std::string& filepath) {
    for (auto loader : _loaders) {
        if (loader != nullptr && loader->CanLoad(filepath)) {
            return loader->Load(filepath);
        }
    }
    std::cout << "ERROR (AssetLoaderManager): Unsupported file extension @ " << filepath << std::endl;
    return false;
}

