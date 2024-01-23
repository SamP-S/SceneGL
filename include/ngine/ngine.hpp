#pragma once

// std libs
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <map>
#include <iostream>
// external libs
#include <entt.hpp>
// internal libs
#include "la_extended.h"
#include "core/uuid.hpp"

//// TODO: the asset N entity engine
// Entities:
//
// Assets:
// Implement asset destruction
// Maybe change naming to find instead of get as not always return asset
// Improve GetAssets to remove iterative conversion/vector build
// Note: Do not


namespace Ngine {

    struct CoreComponent {
        UUID uuid;
        std::string name;
        bool active = true;

        CoreComponent() = default;
        CoreComponent(const CoreComponent&) = default;
        CoreComponent(const std::string& pName)
            : name(pName) {}
    };

    struct TransformComponent {
        LA::vec3 position  = LA::vec3(0.0f);
        LA::vec3 rotation  = LA::vec3(0.0f);
        LA::vec3 scale     = LA::vec3(1.0f);
            
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const LA::vec3& pos)
            : position(pos) {}


        LA::mat4 GetTransform() {
            return LA::Transformation(position, rotation, scale);
        }

        LA::vec3 GetForward() {
            LA::mat4 total = GetTransform();
            return LA::vec3({ total[2][0], total[2][1], total[2][2] });
        }

        LA::vec3 GetRight() {
            LA::mat4 total = GetTransform();
            return LA::vec3({ total[0][0], total[0][1], total[0][2] });
        }

        LA::vec3 GetUp() {
            LA::mat4 total = GetTransform();
            return LA::vec3({ total[1][0], total[1][1], total[1][2] });
        }
        
    };
    
    class Entity;

    class Scene {
        private:
            entt::registry _registry;

            std::vector<Entity> ViewToVector(auto view);

            friend class Entity; 

        public:
            Scene();
            ~Scene();

            Entity CreateEntity(const std::string&);
            void DestroyEntity(Entity);
            void Clear();

            /// TODO: All of the following are slow conversions from entt::view
            /// Implement using internal registry system to support desired functionality
            Entity FindEntityByName(const std::string& name);
            template<typename... Components>
            std::vector<Entity> GetEntitiesWith();
            std::vector<Entity> GetEntities();
            std::size_t GetEntityCount();
            
    };

    class Entity {
        private:
            entt::entity _entityHandle{entt::null};
            Scene* _scene = nullptr;
            
        public:

            // constructors
            Entity() = default;
            Entity(entt::entity handle, Scene* scene)
                : _entityHandle(handle), _scene(scene) {
                    
                }
            Entity(const Entity& other) = default;

            // auto casting
            operator bool() const { return _entityHandle != entt::null; }
            operator entt::entity() const { return _entityHandle; }
		    operator uint32_t() const { return (uint32_t)_entityHandle; }
            bool operator==(const Entity& other) const {
                return _entityHandle == other._entityHandle && _scene == other._scene;
            }
            bool operator!=(const Entity& other) const {
                return !operator==(other);
            }
            
            // check for single component type
            template<typename T>
            bool HasComponent() {
                return _scene->_registry.all_of<T>(_entityHandle);
            }
            // check for multiple component types
            // only true if entity has all of them
            template<typename... Components>
            bool HasComponents() {
                return _scene->_registry.all_of<Components...>(_entityHandle);
            }

            template<typename T>
            T& GetComponent() {
                return _scene->_registry.get<T>(_entityHandle);
            }

            template<typename T>
            T& AddComponent() {
                assert(!HasComponent<T>() && "We already have a component of this type.");
                return _scene->_registry.emplace<T>(_entityHandle);
            }

            template<typename T, typename ...Args>
            T& AddComponent(Args&&... args) {
                assert(!HasComponent<T>() && "We already have a component of this type.");
                return _scene->_registry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
            }
            
            template<typename T>
            void RemoveComponent() {
                static_assert(!std::is_same_v<T, CoreComponent>, "Cannot remove CoreComponent.");
                static_assert(!std::is_same_v<T, TransformComponent>, "Cannot remove TransformComponent.");
                assert(HasComponent<T>() && "We don't have a component of this type to remove.");
                _scene->_registry.remove<T>(_entityHandle);
            }
           
            void Destroy() {
                assert((_scene == nullptr) && "Cannot destroy entity with no scene bound.");
                _scene->DestroyEntity(*this);
                _entityHandle = entt::null;
                _scene = nullptr;
            }

    };

    Scene::Scene() {}
    Scene::~Scene() {}

    std::vector<Entity> Scene::ViewToVector(auto view) {
        std::vector<Entity> arr = std::vector<Entity>(view.size());
        std::size_t idx = 0;
        for (auto entity : view) {
            arr[idx++] = Entity{entity, this};
        }
        return arr;
    }

    Entity Scene::CreateEntity(const std::string& name="Entity") {
        Entity entity = {_registry.create(), this};
        entity.AddComponent<CoreComponent>(name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        _registry.destroy(entity);
    }

    void Scene::Clear() {
        _registry.clear();
    }

    Entity Scene::FindEntityByName(const std::string& name) {
        auto view = _registry.view<CoreComponent>();
        for (auto entity : view) {
            const CoreComponent& nc = view.get<CoreComponent>(entity);
            if (nc.name == name)
                return Entity{entity, this};
        }
        return Entity{entt::null, this};
    }
    template<typename... Components>
    std::vector<Entity> Scene::GetEntitiesWith() {
        auto view = _registry.view<Components...>();
        return ViewToVector(view);
    }
    std::vector<Entity> Scene::GetEntities() {
        auto view = _registry.view<CoreComponent>();
        return ViewToVector(view);
    }

    std::size_t Scene::GetEntityCount() {
        auto view = _registry.view<CoreComponent>();
        return view.size();
    }

    struct Asset {
        public:
            UUID uuid;
            std::string name;
            std::string path;

            Asset() = default;
            Asset(const Asset&) = default;
            Asset(const std::string& name="Asset", const std::string& path="")
                : name(name), path(path) {}
            virtual ~Asset() = default;
    };

    // note: using map as is already sorted when iterating
    class AssetManager {
        private:
            std::map<std::string, uint32_t> _typeToBuffer;
            std::vector<std::map<UUID, uint32_t>> _idToIndex;
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
                    // add uuid look up table
                    std::map<UUID, uint32_t> newLut;
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
            std::vector<IAssetLoader*> _loaders;

        public:
            AssetLoaderManager() {}
            ~AssetLoaderManager() {
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
                std::cout << "ERROR (AssetLoaderManager): Unsupported file extension @ " << filepath << std::endl;
                return false;
            }
    };
    
    // Note: Template base class for serializing scene
    // Specific and unique to each application using Ngine, can't generalise implentation here
    class ISerializer {
        public:
            Ngine::AssetManager &assetManager = Ngine::AssetManager::Instance();
            ISerializer(std::shared_ptr<Scene> scene)
                : _scene(scene) {}

            virtual void Serialize(const std::string &filepath) = 0;
            virtual void Deserialize(const std::string &filepath) = 0;

        protected:
            std::shared_ptr<Scene> _scene;
    };

}
