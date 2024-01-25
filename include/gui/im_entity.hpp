#pragma once

#include <iostream>
#include <string>

#include "gui/im_window.hpp"
#include "ngine/ngine.hpp"
#include "renderer/components.hpp"

//// TODO:
// Implement type LUT to automatically try every panel
// Automate isRequired and title from the component itself

class ImEntity : public IImWindow {
private:
    static inline int _panelCount = 0;

public:
    template<typename T>
    bool ComponentPanelBegin(Entity e, const std::string& title) {
        if (!e.HasComponent<T>())
            return false;

        // support multiple elements with the same label
        ImGui::PushID(_panelCount++);
        // title
        ImGui::Separator();
        ImGui::Text(title.c_str());
        // delete component button
        if constexpr (!std::is_same_v<T, CoreComponent> && !std::is_same_v<T, TransformComponent>) {
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e.RemoveComponent<T>();
            }
        }
        return true;
    }

    void ComponentPanelEnd() {
        ImGui::PopID();
    }

    void EntityWindow(Entity& e) {
        ImGui::Begin("Entity Properties", &isOpen, _windowFlags);
        if (!e) {
            ImGui::Text("Nothing selected");
        } else {
            // components display
            _panelCount = 0;
            if (ImGui::Button("Delete Entity")) {
                e.Destroy();
            } else {
                // name/enable/id
                CorePanel(e);
                // transform
                TransformPanel(e);
                // non-singlton components
                MeshRendererPanel(e);
                CameraPanel(e);
                DirectionalLightWindow(e);
                PointLightWindow(e);
                SpotLightWindow(e);
            }
        }
        ImGui::End();
    }
    
    void CorePanel(Entity& e) {
        if (!ComponentPanelBegin<CoreComponent>(e, "Entity")) {
            std::cout << "ERROR (App): Missing core component." << std::endl;
            return;
        }
            
        CoreComponent& cc = e.GetComponent<CoreComponent>();
        // ID
        ImGui::Text(std::to_string(cc.uuid).c_str());
        // entity name
        ImGui::Text("Name:");
        ImGui::SameLine();
        char buffer[256];
        std::strncpy(buffer, cc.name.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0; // Ensure null termination
        if (ImGui::InputText("##NameEntity", buffer, sizeof(buffer))) {
            cc.name = std::string(buffer);
        }
        ImGui::Checkbox("Active", &cc.active);
        ComponentPanelEnd();
    }

    void TransformPanel(Entity& e) {
        if (!ComponentPanelBegin<TransformComponent>(e, "Transform")) {
            std::cout << "ERROR (App): Missing transform component." << std::endl;
            return;
        }

        TransformComponent& t = e.GetComponent<TransformComponent>();
        // position
        ImGui::Text("Pos");
        ImGui::SameLine();
        ImGui::InputFloat3("##PosInput", t.position.m);
        // rotation
        ImGui::Text("Rot");
        ImGui::SameLine();
        ImGui::InputFloat3("##RotInput", t.rotation.m);
        // scale
        ImGui::Text("Scl");
        ImGui::SameLine();
        ImGui::InputFloat3("##SclInput", t.scale.m);

        ComponentPanelEnd();
    }


    void MeshRendererPanel(Entity& e) {
        if (!ComponentPanelBegin<MeshRendererComponent>(e, "Mesh Renderer"))
            return;

        MeshRendererComponent& mrc = e.GetComponent<MeshRendererComponent>();
        AssetManager& assetManager = AssetManager::Instance();

        if (ImGui::BeginCombo("Select Mesh", ((mrc.mesh == nullptr) ? "None": mrc.mesh->name.c_str()))) {
            if (ImGui::Selectable("None")) {
                mrc.mesh = nullptr;
            }
            for (auto it = assetManager.begin<OpenGLMesh>(); it != assetManager.end<OpenGLMesh>(); ++it) {
                std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(*it);
                if (ImGui::Selectable(mesh->name.c_str())) {
                    mrc.mesh = mesh;
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Select Material", ((mrc.material == nullptr) ? "None": mrc.material->name.c_str()))) {
            if (ImGui::Selectable("None")) {
                mrc.material = nullptr;
            } 
            for (auto it = assetManager.begin<OpenGLMaterial>(); it != assetManager.end<OpenGLMaterial>(); ++it) {
                std::shared_ptr<Material> material = std::dynamic_pointer_cast<Material>(*it);
                if (ImGui::Selectable(material->name.c_str())) {
                    mrc.material = material;
                }
            }
            ImGui::EndCombo();
        }

        ComponentPanelEnd();
    }

    void CameraPanel(Entity& e) {
        if (!ComponentPanelBegin<CameraComponent>(e, "Camera"))
            return;
        CameraComponent& cc = e.GetComponent<CameraComponent>();
        ImGui::SliderFloat("FOV", &cc.fov, 45.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoRoundToFormat);
        ComponentPanelEnd();
    }

    void DirectionalLightWindow(Entity e) {
        if (!ComponentPanelBegin<DirectionalLightComponent>(e, "Directional Light"))
            return;
        DirectionalLightComponent& dlc = e.GetComponent<DirectionalLightComponent>();
        ImGui::InputFloat3("Colour", dlc.colour.m);
        ImGui::SliderFloat("Intensity", &dlc.intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        ComponentPanelEnd();
    }

    void PointLightWindow(Entity& e) {
        if (!ComponentPanelBegin<PointLightComponent>(e, "Point Light"))
            return;
        PointLightComponent& plc = e.GetComponent<PointLightComponent>();
        ImGui::InputFloat3("Colour", plc.colour.m);
        ImGui::SliderFloat("Intensity", &plc.intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::SliderFloat("Range", &plc.range, 0.01f, 20.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);

        ComponentPanelEnd();
    }

    void SpotLightWindow(Entity& e) {
        if (!ComponentPanelBegin<SpotLightComponent>(e, "Spot Light"))
            return;
        SpotLightComponent& slc = e.GetComponent<SpotLightComponent>();
        ImGui::InputFloat3("Colour", slc.colour.m);
        ImGui::SliderFloat("Intensity", &slc.intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::SliderFloat("Cut Off", &slc.cutOff, 5.0f, 15.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::SliderFloat("Outer Cut Off", &slc.outerCutOff, 5.0f, 90.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
        ComponentPanelEnd();
    }

};

