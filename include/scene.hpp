#include "object.hpp"
#include "entity.hpp"

class Scene : public Object {
private:
    std::vector<Entity*> _entities;
    std::string _name = "Default Scene";

public:
    Scene(std::string name = "Default Scene") :
        _name(name) {}

    std::string Type() {
        return "Scene";
    }

    // JSON
    void FromJson(json j) {
        for (const auto& entJson : j) {
            // create entity
            Entity* ent = new Entity();
            ent->FromJson(entJson);
            _entities.push_back(ent);
        }
    }

    json ToJson() {
        json j;
        for (auto& ent : _entities) {
            j.push_back(ent->ToJson());
        }
        return j;
    }

    std::string ToString() {
        return "Scene(" + _name + ")";
    }

    // make iterable
    auto begin() {
        return _entities.begin();
    }

    auto end() {
        return _entities.end();
    }

    Entity* at(int i) {
        return operator[](i);
    }

    int size() {
        return _entities.size();
    }

    // name
    std::string GetName() {
        return _name;
    }
    void SetName(std::string name) {
        _name = name;
    }

    // index operator
    Entity* operator[](int i) {
        if (i < _entities.size()) {
            return _entities[i];
        }
        return nullptr;
    }
};