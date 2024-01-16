#pragma once

#include <iostream>

#include "component.hpp"
#include "mesh.hpp"
#include "entity.hpp"

class MeshRenderer : public Component {
    public:
        MeshRenderer(Entity* entity, ObjId id=0) :
            Component(entity),
            _meshId(id) {}

        std::string Type() {
            return "MeshRenderer";
        }

        void FromJson(json j) {
            _meshId = resourceMeshes.GetId(j["meshName"]);
        }

        json ToJson() {
            json j;
            j["meshRenderer"]["meshName"] = resourceMeshes.Get(_meshId)->GetName();
            return j;
        }

        std::string ToString() {
            std::string s = "MeshRenderer(";
            s += resourceMeshes.Get(_meshId)->GetName() + ")";
            return s;
        }

        ObjId GetMeshId() {
            return _meshId;
        }

        void SetMeshId(ObjId id) {
            _meshId = id;
        }

    private:
        ObjId _meshId;
};
