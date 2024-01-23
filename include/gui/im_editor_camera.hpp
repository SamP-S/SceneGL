#pragma once

#include "gui/im_window.hpp"
#include "renderer/editor_camera.hpp"

class ImEditorCamera : public IImWindow {
public:
    static void EditorCameraWindow(bool* isOpen, std::shared_ptr<EditorCamera> editorCamera) {
        ImGui::Begin("Editor Camera", isOpen, _windowFlags);
        ImGui::Text("Camera");
        ImGui::Separator();
        ImGui::Text("Transform");
        ImGui::End();
    }
};
