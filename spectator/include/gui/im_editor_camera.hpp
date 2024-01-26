#pragma once

#include <string>

#include "gui/im_window.hpp"
#include "renderer/editor_camera.hpp"

#include "ImGuizmo.h"

class ImEditorCamera : public IImWindow {
    
    static inline std::map<std::string, float> _labelToAR = {
        {"None", 0.0f},
        {"16:9", 16.0f / 9.0f},
        {"5:4", 5.0f / 4.0f},
        {"4:3", 4.0f / 3.0f},
        {"1:1", 1.0f}
    };

    static inline std::map<float, std::string> _ARToLabel = {
        {0.0f, "None"},
        {16.0f / 9.0f, "16:9"},
        {5.0f / 4.0f, "5:4"},
        {4.0f / 3.0f, "4:3"},
        {1.0f, "1:1"}
    };

    void PerspectivePanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::SliderFloat("Fov", &ec->fov, 45.f, 110.f);
    }

    void OrthographicPanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::SliderFloat("Ortho Width", &ec->orthoWidth, 1, 100);
        ImGui::SliderFloat("Ortho Height", &ec->orthoHeight, 1, 100);
    }

    void CameraPanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::Text("Camera");
        if (ImGui::BeginCombo("Aspect Ratio", _ARToLabel[ec->aspectRatio].c_str())) {
            for (auto const& ar : _labelToAR) {
                if (ImGui::Selectable(ar.first.c_str()))
                    ec->aspectRatio = ar.second;
            }
            ImGui::EndCombo();
        }
        ImGui::SliderFloat("Near Plane", &ec->near, 0.01f, 100);
        ImGui::SliderFloat("Far Plane", &ec->far, 1.0f, 100);

        if (ImGui::RadioButton("Perspective", ec->isPerspective))
            ec->isPerspective = true;
        ImGui::SameLine();
        if (ImGui::RadioButton("Orthographic", !ec->isPerspective))
            ec->isPerspective = false;

        // create correct perspective elements
        // do not merge with imguizmo
        if (ec->isPerspective)
            PerspectivePanel(ec);
        else
            OrthographicPanel(ec);
        
        // ImGuizmo
        ImGuizmo::SetOrthographic(!ec->isPerspective);
        ImGuizmo::BeginFrame();
        
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
        if (ImGuizmo::IsUsing()) {
            ImGui::Text("Using gizmo");
        } else {
            ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
            ImGui::SameLine();
            ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
            ImGui::SameLine();
            ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
            ImGui::SameLine();
            ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
        }
    }

    void GuizmoPanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::Text("Guizmo");
        // guizmo operation selecton
        if (ImGui::RadioButton("Translate", guizmoOp == ImGuizmo::TRANSLATE))
            guizmoOp = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", guizmoOp == ImGuizmo::ROTATE))
            guizmoOp = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", guizmoOp == ImGuizmo::SCALE))
            guizmoOp = ImGuizmo::SCALE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Universal", guizmoOp == ImGuizmo::UNIVERSAL))
            guizmoOp = ImGuizmo::UNIVERSAL;

        // local or world coordinate spacing?
        if (ImGui::RadioButton("Local", guizmoCoord == ImGuizmo::LOCAL))
            guizmoCoord = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", guizmoCoord == ImGuizmo::WORLD))
            guizmoCoord = ImGuizmo::WORLD;
    }

    void TransformPanel(std::shared_ptr<EditorCamera> ec) {
        // camera transform
        ImGui::Text("Transform");
        ImGui::InputFloat3("Pos", ec->transform.position.m);
        ImGui::InputFloat3("Rot", ec->transform.rotation.m);
        ImGui::InputFloat3("Scl", ec->transform.scale.m);
    }

public:
    // debug
    ImGuizmo::OPERATION guizmoOp = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE guizmoCoord = ImGuizmo::WORLD;

    void EditorCameraWindow(std::shared_ptr<EditorCamera> editorCamera) {
        ImGui::Begin("Editor Camera", &isOpen, _windowFlags);
        CameraPanel(editorCamera);
        ImGui::Separator();
        GuizmoPanel(editorCamera);
        ImGui::Separator();
        TransformPanel(editorCamera);
        ImGui::End();
    }
};
