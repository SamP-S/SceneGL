#pragma once

#include <iostream>

#include "component.hpp"
#include "mesh.hpp"
#include "entity.hpp"

class MeshRenderer : public Component {
    public:
        MeshRenderer(Entity& entity, ObjId id=0) :
            Component(entity),
            _meshId(id) {}

        ObjId GetMeshId() {
            return _meshId;
        }

        void SetMeshId(ObjId id) {
            _meshId = id;
        }

        bool Render() {
            if (_meshId != 0) {
                Mesh* mesh = resourceMeshes.Get(_meshId);
                if (mesh == NULL) {
                    std::cout << "WARNING: Tried rendering NULL mesh." << std::endl;
                    return false;;
                }
                mesh->Render();
                return true;
            }
        }

        std::string ComponentType() {
            return "MeshRenderer";
        }

    private:
        ObjId _meshId;
};
