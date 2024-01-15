#include "component.hpp"

/// TODO: Switch to references instead of pointers

template<typename T>
T* Entity::GetComponent() {
    for (auto component : _components) {
        if (dynamic_cast<T*>(component) != nullptr) {
            return dynamic_cast<T*>(component);
        }
    }
    return nullptr;
}

template<typename T>
std::vector<T*> Entity::GetComponents() {
    std::vector<T*> results  = std::vector<T*>();
    for (auto component : _components) {
        if (dynamic_cast<T*>(component) != nullptr) {
            results.push_back(dynamic_cast<T*>(component));
        }
    }
    return results;
}

template<typename T>
T* Entity::GetComponentInChildren() {
    T* childComp = GetComponent<T>();
    if (childComp != nullptr) {
        return childComp;
    }

    for (auto child : _children) {
        childComp = child->GetComponentInChildren<T>();
        if (childComp != nullptr) {
            return childComp;
        }
    }
    return nullptr;
}

template<typename T>
std::vector<T*> Entity::GetComponentsInChildren() {
    std::vector<T*> results = GetComponents<T>();
    for (auto child : _children) {
        std::vector<T*> childComponents = child->GetComponentsInChildren<T>();
        results.insert(results.end(), childComponents.begin(), childComponents.end());
    }
    return results;
}

template<typename T>
T* Entity::AddComponent() {
    T* c = new T(*this);
    try {
        _components.push_back(static_cast<Component*>(c));
        return c;
    } catch (const std::exception& e) {
        std::cout << "ERROR (Entity): Can't add component: " << e.what() << std::endl;
        delete c; // Clean up the allocated memory
        return nullptr;
    }
}

template<typename T>
bool Entity::RemoveComponent(T* key) {
    if (key == nullptr) {
        std::cout << "WARNING (Entity): Trying to remove NULL component." << std::endl;
        return false;
    }
    auto it = std::find(_components.begin(), _components.end(), key);
    if (it == _components.end()) {
        std::cout << "WARNING (Entity): Trying to remove component not owned by entity." << std::endl;
        return false;
    }
    _components.erase(it);
    delete key;
    return true;
}