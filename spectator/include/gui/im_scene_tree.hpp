#pragma once

#include "gui/im_window.hpp"
#include "ecs/ngine.hpp"

//// TODO:
// Implement as tree
// Allow for drag and dropping of nodes to rebind tree structure

class ImSceneTree : public IImWindow {
public:
    Entity entitySelected;

    bool HasSelected() {
        return (bool)entitySelected;
    }

    // flattened until scene tree implemented
    void SceneNode(Entity e) {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        if (entitySelected == e)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        node_flags |= ImGuiTreeNodeFlags_Leaf;
        bool node_open = ImGui::TreeNodeEx(e.GetComponent<CoreComponent>().name.c_str(), node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            entitySelected = e;
        if (node_open) {
            ImGui::TreePop();
        }   
    }
    
    void SceneTreeWindow(std::shared_ptr<Scene> scene) {
        ImGui::Begin("Scene Tree", &isOpen, _windowFlags);
        if (scene != nullptr) {
            std::vector<Entity> entities = scene->GetEntities();
            for (auto e : entities) {
                SceneNode(e);
            }
        }
        ImGui::End();
    }

};
