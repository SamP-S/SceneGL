#pragma once

// std libs
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <map>
#include <iostream>
#include <memory>
#include <algorithm>

// internal libs
#include "core/uuid.hpp"

// Assets:
// Implement asset destruction
// Maybe change naming to find instead of get as not always return asset
// Improve GetAssets to remove iterative conversion/vector build
// Rename asset manager to asset library
// Consolidate asset manager & asset loader manager into single interface/class, use composition & singleton
// Fix singletons to remove copy and assignment operators
// set singleton destructors protected if object contains smart ptrs

struct Asset {
    public:
        UUID uuid;
        std::string name;
        std::string path;

        Asset() = default;
        Asset(const Asset&) = default;
        Asset(const std::string& name="Asset", const std::string& path="");
        virtual ~Asset() = default;
};

// note: using map as is already sorted when iterating
class AssetManager {
    private:
        std::map<std::string, uint32_t> _typeToBuffer;
        std::vector<std::map<UUID, uint32_t>> _idToIndex;
        std::vector<std::vector<std::shared_ptr<Asset>>> _assetLibrary;

        // private constructor for singleton
        AssetManager() = default;
        ~AssetManager() = default;

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
                // add uuid look up table
                std::map<UUID, uint32_t> newLut;
                _idToIndex.push_back(newLut);
            }
        }

    public:
        // Public static method to get the single instance
        static AssetManager& Instance();

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
            _idToIndex[buf].emplace(newAsset->uuid, idx);
            _assetLibrary[buf].push_back(newAsset);
            return newAsset;
        }

        template<typename T>
        std::shared_ptr<T> GetAssetByUUID(UUID uuid) {
            std::string s = typeid(T).name();
            // didnt find buffer to even hold T, return null
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                return nullptr;
            }
            uint32_t buf = _typeToBuffer[s];
            // couldnt find uuid in index map
            if (_idToIndex[buf].find(uuid) == _idToIndex[buf].end()) {
                return nullptr;
            }
            uint32_t idx = _idToIndex[buf][uuid];
            std::shared_ptr<Asset> ptr = _assetLibrary[buf][idx];
            return std::dynamic_pointer_cast<T>(ptr);
        }

        template<typename T>
        std::shared_ptr<T> FindAsset(const std::string& name) {
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
        UUID GetAssetUUID(const std::string& name) {
            std::string s = typeid(T).name();
            // didnt find buffer to even hold T, return null
            if (_typeToBuffer.find(s) == _typeToBuffer.end()) {
                return 0;
            }
            std::vector<std::shared_ptr<Asset>> arr = GetAssets<T>();
            for (auto asset : arr) {
                if (asset->name.compare(name) == 0)
                    return asset->uuid;
            }
            // could not find matching name
            return 0;
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

class AssetLoaderManager {
    private:
        std::vector<std::shared_ptr<IAssetLoader>> _loaders;

        AssetLoaderManager() = default;
        ~AssetLoaderManager() = default;

    public:
        // Public static method to get the single instance
        static AssetLoaderManager& Instance();

        void AddLoader(std::shared_ptr<IAssetLoader> loader);

        bool Load(const std::string& filepath);
};
