#pragma once

// std libs
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
// external libs
#include <entt.hpp>
// internal
#include "components.hpp"

namespace Ngine {
    
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

            /// TODO: All of the following are slow conversions from entt::view
            /// Implement using internal registry system to support desired functionality
            Entity FindEntityByName(const std::string& name);
            template<typename... Components>
            std::vector<Entity> GetEntitiesWith();
            std::vector<Entity> GetEntities();
            
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

    Entity Scene::CreateEntity(const std::string& name=std::string("Name")) {
        Entity entity = {_registry.create(), this};
        entity.AddComponent<CoreComponent>(name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        _registry.destroy(entity);
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

}
