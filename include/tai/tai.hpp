#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <map>
#include <typeinfo>

#include "core/filepath.hpp"
#include "ngine/object.hpp"

namespace Tai {

    class Asset : public Object {
    public:
        std::string name;
        std::string path;

        Asset() = default;
        Asset(const Asset&) = default;
        Asset(const std::string& name="Asset", const std::string& path="")
            : name(name), path(path) {}

    };

    class IAssetLoader {
    private:
        std::vector<std::string> _extensions;
        
    public:
        virtual bool Load(const std::string& filepath) = 0;
        virtual std::vector<std::string> GetSupportedExt() = 0;
        virtual bool CanLoad(const std::string& filepath) = 0;
        
    };

    class LoaderManager : public Object {
    private:
        std::vector<IAssetLoader*> _loaders;

    public:
        LoaderManager() {}

        ~LoaderManager() {
            // clear loaders
            for (IAssetLoader* loader : _loaders) {
                delete loader;
            }
            _loaders.clear();
        }

        void AddLoader(IAssetLoader* loader) {
            _loaders.push_back(loader);
        }

        void RemoveLoader(IAssetLoader* loader) {
            auto it = std::find(_loaders.begin(), _loaders.end(), loader);
            if (it != _loaders.end()) {
                _loaders.erase(it);
            }
        }

        bool Load(const std::string& filepath) {
            for (auto loader : _loaders) {
                if (loader != nullptr && loader->CanLoad(filepath)) {
                    return loader->Load(filepath);
                }
            }
            std::cout << "ERROR (LoaderManager): Unsupported file extension @ " << filepath << std::endl;
            return false;
        }
    };

    class AssetManager : public Object {
    private:
        std::map<std::string, uint32_t> _typeToBuffer;
        std::vector<std::map<ObjectId, uint32_t>> _idToIndex;
        std::vector<std::vector<std::shared_ptr<Asset>>> _assetLibrary;

    public:
        AssetManager() {}
        ~AssetManager() {}

        template<typename T, typename... Args>
        std::shared_ptr<T> CreateAsset(Args&&... args) {
            std::string s = typeid(T).name();
            uint32_t buf = -1;
            // didnt find buffer for T, create new buffer
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                buf = _assetLibrary.size();
                _typeToBuffer[s] = buf;
                std::vector<std::shared_ptr<Asset>> newBuffer;
                _assetLibrary.push_back(newBuffer);
            // otherwise just set buffer index
            } else {
                buf = _idToIndex[_typeToBuffer[s]];
            }
            std::shared_ptr<T> newAsset = std::make_shared<T>(std::forward<Args>(args)...);
            _idToIndex[buf][newAsset->id] = _assetLibrary[buf].size();
            _assetLibrary[buf].push_back(newAsset);
            return newAsset;
        }

        template<typename T>
        std::shared_ptr<T> GetAsset(ObjectId id) {
            std::string s = typeid(T).name();
            // didnt find buffer to even hold T, return null
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                return nullptr;
            }
            uint32_t buf = _typeToBuffer[s];
            // couldnt find id in index map
            if (_idToIndex[buf].find(id) == _idToIndex[buf].end()) {
                return nullptr;
            }
            uint32_t idx = _idToIndex[buf][id];
            return _assetLibrary[buf][idx];
        }

        template<typename T>
        std::vector<std::shared_ptr<Asset>> GetAssets() {
            std::string s = typeid(T).name();
            // check for buffer of T
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                return std::vector<std::shared_ptr<Asset>>();
            }
            return _assetLibrary[_typeToBuffer[s]];
        }
        
        /// TODO: Implement asset destruction
    };

}






