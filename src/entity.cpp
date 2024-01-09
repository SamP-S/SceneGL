#pragma once

#include "entity.h"


Entity::Entity(std::string name, Entity* parent) :
    _name(name),
    _parent(parent) {
        _entityId = Entity::_nextId++;
    };

std::string Entity::GetName() {
    return _name;
}

void Entity::SetName(std::string name) {
    _name = name;
}

void Entity::AddChild(Entity* child) {
    _children.push_back(child);
}

int Entity::GetNumChildren() {
    return _children.size();
}

Entity* Entity::GetChild(int index) {
    return _children.at(index);
}

Entity* Entity::FindChild(std::string key) {
    return GetChild(FindChildIndex(key));
}

int Entity::FindChildIndex(std::string key) {
    for (size_t i = 0; i < _children.size(); i++) {
        const auto& child = _children[i];
        if (child->GetName().compare(key) == 0) {
            return i;
        }
    }
    return -1;
}

int Entity::FindChildIndex(Entity* key) {
    for (size_t i = 0; i < _children.size(); i++) {
        const auto& child = _children[i];
        if (child == key) {
            return i;
        }
    }
    return -1;
}

bool Entity::IsRoot() {
    return _parent == NULL;
}

Entity* Entity::GetParent() {
    return _parent;
}

void Entity::SetParent(Entity* parent) {
    if (_parent != NULL) {
        _parent->RemoveChild(this);
    }
    _parent = parent;
}

int Entity::GetNumComponents() {
    return _components.size();
}

void Entity::AddComponent(Component* component) {
    if (component == nullptr) {
        std::cout << "WARNING (Entity): Trying to add NULL component." << std::endl;
        return;
    }
    _components.push_back(component);
}

void Entity::RemoveComponent(Component* key) {
    if (key == nullptr) {
        std::cout << "WARNING (Entity): Trying to remove NULL component." << std::endl;
        return;
    }
    auto it = std::find(_components.begin(), _components.end(), key);
    if (it != _components.end()) {
        _components.erase(it);
    }
}

bool Entity::RemoveChild(int index) {
    if (index < 0 || index >= GetNumChildren()) {
        std::cout << "WARNING (Entity): Trying to remove child of invalid index." << std::endl;
        return false;
    }
    _children.erase(_children.begin() + index);
    return true;
}

bool Entity::RemoveChild(Entity* key) {
    return RemoveChild(FindChildIndex(key));
}


EntityId Entity::_nextId = 0;