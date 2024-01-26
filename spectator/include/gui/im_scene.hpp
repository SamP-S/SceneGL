#pragma once

#include "gui/im_window.hpp"
#include "ngine/ngine.hpp"

//// TODO:
// Implement as tree
// Allow for drag and dropping of nodes to rebind tree structure

class ImScene : public IImWindow {
public:
    // flattened until scene tree implemented
    void SceneNode(Entity& selected, Entity e) {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        if (selected == e)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        node_flags |= ImGuiTreeNodeFlags_Leaf;
        bool node_open = ImGui::TreeNodeEx(e.GetComponent<CoreComponent>().name.c_str(), node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            selected = e;
        if (node_open) {
            ImGui::TreePop();
        }   
    }
    
    void SceneWindow(std::shared_ptr<Scene> scene, Entity& selected) {
        ImGui::Begin("Scene Tree", &isOpen, _windowFlags);
        if (scene != nullptr) {
            std::vector<Entity> entities = scene->GetEntities();
            for (auto e : entities) {
                SceneNode(selected, e);
            }
        }
        ImGui::End();
    }

};
