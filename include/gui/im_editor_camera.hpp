#pragma once

#include <string>

#include "gui/im_window.hpp"
#include "renderer/editor_camera.hpp"

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

    static void PerspectivePanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::SliderFloat("Fov", &ec->fov, 20.f, 110.f);
    }

    static void OrthographicPanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::SliderFloat("Ortho Width", &ec->orthoWidth, 1, 20);
        ImGui::SliderFloat("Ortho Height", &ec->orthoHeight, 1, 20);
    }

    static void CameraPanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::Text("Camera");
        if (ImGui::BeginCombo("Aspect Ratio", _ARToLabel[ec->aspectRatio].c_str())) {
            for (auto const& ar : _labelToAR) {
                if (ImGui::Selectable(ar.first.c_str()))
                    ec->aspectRatio = ar.second;
            }
            ImGui::EndCombo();
        }

        if (ImGui::RadioButton("Perspective", ec->isPerspective))
            ec->isPerspective = true;
        ImGui::SameLine();
        if (ImGui::RadioButton("Orthographic", !ec->isPerspective))
            ec->isPerspective = false;

        if (ec->isPerspective)
            PerspectivePanel(ec);
        else
            OrthographicPanel(ec);
    }

    static void TransformPanel(std::shared_ptr<EditorCamera> ec) {
        ImGui::Text("Transform");
    }

public:
    static void EditorCameraWindow(bool* isOpen, std::shared_ptr<EditorCamera> editorCamera) {
        ImGui::Begin("Editor Camera", isOpen, _windowFlags);
        CameraPanel(editorCamera);
        ImGui::Separator();
        TransformPanel(editorCamera);
        ImGui::End();
    }
};
