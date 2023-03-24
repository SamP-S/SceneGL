#pragma once

#include <vector>
#include <string>

#include "entity.hpp"

class SceneNode {

    public:

        SceneNode(EntityId entityId, SceneNode* parentId)
        : _entityId(entityId),
        _parent(parentId) {}

        void AddChild(SceneNode* childId) {
            _children.push_back(childId);
        }

        int GetNumChildren() {
            return _children.size();
        }

        SceneNode* GetChild(int index) {
            return _children.at(index);
        }

        SceneNode* RemoveChild(int index) {
            SceneNode* childId = GetChild(index);
            _children.erase(_children.begin() + index);
            return childId;
        }

        void SetParent(SceneNode* parent) {
            _parent = parent;
        }

        EntityId GetEntity() {
            return _entityId;
        }

    private:
        std::vector<SceneNode*> _children = std::vector<SceneNode*>();
        EntityId _entityId = -1;
        SceneNode* _parent = NULL;
};