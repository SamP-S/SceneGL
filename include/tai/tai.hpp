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

    struct Asset : public Object {
    public:
        std::string name;
        std::string path;

        Asset() = default;
        Asset(const Asset&) = default;
        Asset(const std::string& name="Asset", const std::string& path="")
            : name(name), path(path) {}
        virtual ~Asset() = default;
    };

    class AssetManager : public Object {
    private:
        std::map<std::string, uint32_t> _typeToBuffer;
        std::vector<std::map<ObjectId, uint32_t>> _idToIndex;
        std::vector<std::vector<std::shared_ptr<Asset>>> _assetLibrary;

        // private constructor for singleton
        AssetManager() {}
        ~AssetManager() {}

        // Delete copy constructor and assignment operator
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        template<typename T>
        void CheckForBuffer() {
            std::string s = typeid(T).name();
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                uint32_t buf = _assetLibrary.size();
                _typeToBuffer[s] = buf;
                // add buffer
                std::vector<std::shared_ptr<Asset>> newBuffer;
                _assetLibrary.push_back(newBuffer);
                // add id look up table
                std::map<ObjectId, uint32_t> newLut;
                _idToIndex.push_back(newLut);
            }
        }

    public:
        // Public static method to get the single instance
        static AssetManager& Instance() {
            static AssetManager instance;
            return instance;
        }

        template<typename T>
        auto begin() {
            CheckForBuffer<T>();
            std::string s = typeid(T).name();
            uint32_t buf = _typeToBuffer[s];
            return _assetLibrary[buf].begin();
        }

        template<typename T>
        auto end() {
            CheckForBuffer<T>();
            std::string s = typeid(T).name();
            uint32_t buf = _typeToBuffer[s];
            return _assetLibrary[buf].end();
        }

        template<typename T, typename... Args>
        std::shared_ptr<T> CreateAsset(Args&&... args) {
            CheckForBuffer<T>();
            std::string s = typeid(T).name();
            uint32_t buf = _typeToBuffer[s];
            std::shared_ptr<T> newAsset = std::make_shared<T>(std::forward<Args>(args)...);
            uint32_t idx = _assetLibrary[buf].size();
            _idToIndex[buf].emplace(newAsset->id, idx);
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
            std::shared_ptr<Asset> ptr = _assetLibrary[buf][idx];
            return std::dynamic_pointer_cast<T>(ptr);
        }

        template<typename T>
        std::shared_ptr<T> GetAsset(const std::string& name) {
            std::string s = typeid(T).name();
            // didnt find buffer to even hold T, return null
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                return nullptr;
            }
            std::vector<std::shared_ptr<Asset>> arr = GetAssets<T>();
            for (auto asset : arr) {
                if (asset->name.compare(name) == 0)
                    return std::dynamic_pointer_cast<T>(asset);
            }
            // could not find matching name
            return nullptr;
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

        class IAssetLoader {
    private:
        std::vector<std::string> _extensions;
        
    public:
        AssetManager&  assetManager = AssetManager::Instance();
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

}






