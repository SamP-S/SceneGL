#pragma once

#include "entity.hpp"
#include "light_directional.hpp"
#include "light_point.hpp"
#include "mesh_renderer.hpp"

Entity::Entity(std::string name, Entity* parent) :
    Object(),
    _name(name),
    _parent(parent) {}


// JSON
/// TODO: Derive parent from JSON

std::string Entity::Type() {
    return "Entity";
}

void Entity::FromJson(json j) {
    // parse entity
    _name = j["name"];
    json components = j["components"];
    transform.FromJson(j["transform"]);
    // create component(s)
    for (const auto& component : components) {
        if (component.find("directionalLight") != component.end()) {
            DirectionalLight* c = new DirectionalLight(*this);
            c->FromJson(component["directionalLight"]);
            AddComponent(c);
        } else if (component.find("pointLight") != component.end()) {
            PointLight* c = new PointLight(*this);
            c->FromJson(component["pointLight"]);
            AddComponent(c);
        }
        else if (component.find("meshRenderer") != component.end()) {
            MeshRenderer* c = new MeshRenderer(*this);
            c->FromJson(component["meshRenderer"]);
            AddComponent(c);
        }
    }
}

json Entity::ToJson() {
    json j;
    j["name"] = _name;
    j["transform"] = transform.ToJson();
    json components;
    for (const auto& component : _components) {
        if (auto directionalLight = dynamic_cast<DirectionalLight*>(component)) {
            components.push_back(directionalLight->ToJson());
        } else if (auto pointLight = dynamic_cast<PointLight*>(component)) {
            components.push_back(pointLight->ToJson());
        } else if (auto meshRenderer = dynamic_cast<MeshRenderer*>(component)) {
            components.push_back(meshRenderer->ToJson());
        }
    }
    j["components"] = components;
    return j;
}

std::string Entity::ToString() {
    return "Entity()" + _name;
}

// name attribute 
std::string Entity::GetName() {
    return _name;
}

void Entity::SetName(std::string name) {
    _name = name;
}

// components
int Entity::GetNumComponents() {
    return _components.size();
}

// parent attribute
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

// entity tree/children
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