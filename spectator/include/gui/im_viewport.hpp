#pragma once

#include "gui/im_window.hpp"
#include "renderer/frame_buffer.hpp"
#include "renderer/editor_camera.hpp"
#include "gui/im_editor_camera.hpp"

//// TODO:
// Generalise viewport to draw any texture, not necessarily a frame buffer
// Should use aspect ratio of texture to ensure it is not stretched
// Find a way to keep original resolution/downsample better
// Replace imvec2 with LA::vec2?
// Fix rotation breaking

class ImViewport : public IImWindow {
public:
    ImVec2 render_region_min = ImVec2();
    ImVec2 render_region_max = ImVec2();
    bool isFocuesed = false;
    float aspectRatio = 0.0f;

    ImVec2 AspectRatioLock(const ImVec2 maxSize, float aspectRatio) {
        float maxAspectRatio = maxSize.x / maxSize.y;
        ImVec2 wSize = maxSize;
        if (aspectRatio != 0) {
            if (aspectRatio >= maxAspectRatio)
                wSize.y = wSize.x / aspectRatio;
            else if (aspectRatio < maxAspectRatio)
                wSize.x = wSize.y * aspectRatio;
        }
        return wSize;
    }

    ImVec2 GetWindowRelative(ImVec2 globalCoord, bool clamp=true) {
        int x = globalCoord.x - render_region_min.x;
        int y = globalCoord.y - render_region_min.y;
        if (!clamp)
            return ImVec2(x, y);
        
        // clamp to render region limits
        x = ((x >= 0) ? x : 0);
        y = ((y >= 0) ? y : 0);
        x = ((x <= render_region_max.x) ? x : render_region_max.x);
        y = ((y <= render_region_max.y) ? y : render_region_max.y);
        return ImVec2(x, y);
    }

    void ViewportWindow(std::shared_ptr<FrameBuffer> fb, std::shared_ptr<EditorCamera> ec, Entity& entitySelected, ImEditorCamera& imEC) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport Window", &isOpen, _windowFlags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::PopStyleVar(1);

        isFocuesed = ImGui::IsWindowFocused();

        // content size
        ImVec2 globalPos = ImGui::GetWindowPos();
        ImVec2 relativePos = ImVec2();
        if (ImGui::GetCurrentWindow()->ParentWindow) {
            ImVec2 parent_pos = ImGui::GetCurrentWindow()->ParentWindow->Pos;
            relativePos.x = globalPos.x - parent_pos.x;
            relativePos.y = globalPos.y - parent_pos.y;
        } else {
            relativePos = globalPos;
        }
        render_region_min = ImGui::GetWindowContentRegionMin();
        render_region_min.x += relativePos.x;
        render_region_min.y += relativePos.y;
        render_region_max = ImGui::GetWindowContentRegionMax();
        render_region_max.x += relativePos.x;
        render_region_max.y += relativePos.y;

        // std::cout << "reg min " << render_region_min.x << ", " << render_region_min.y << std::endl;
        // std::cout << "reg max " << render_region_max.x << ", " << render_region_max.y << std::endl;
        
        // ImVec2 res = (ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin()) + ImGui::GetWindowPos() - window_pos;
        float wWidth = render_region_max.x - render_region_min.x;
        float wHeight = render_region_max.y - render_region_min.y;
        ImVec2 wSize = AspectRatioLock(ImVec2(wWidth, wHeight), aspectRatio);

        // draw frame to viewport
        ImGui::Image((ImTextureID)fb->GetColourAttachment(), wSize, ImVec2(0, 1), ImVec2(1, 0));



        // imguizmo
        if (entitySelected) {
            ImGuiIO& io = ImGui::GetIO();
            float viewManipulateRight = io.DisplaySize.x;
            float viewManipulateTop = 0;
            static ImGuiWindowFlags gizmoWindowFlags = 0;

            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
            viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
            viewManipulateTop = ImGui::GetWindowPos().y;
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

            LA::mat4 view = LA::inverse(ec->transform.GetTransform());
            LA::mat4 proj = ec->GetProjection();
            
            TransformComponent& tc = entitySelected.GetComponent<TransformComponent>();
            LA::mat4 selectedTrans = tc.GetTransform();

            float distance = 8.f;
            ImGuizmo::ViewManipulate((float*)&view, distance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
            
            // ImGuizmo::Manipulate((float*)&cameraView, (float*)&cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
            if (ImGuizmo::Manipulate((float*)&view, (float*)&proj, imEC.guizmoOp, imEC.guizmoCoord, (float*)&selectedTrans, NULL, NULL, NULL, NULL)) {
                std::cout << "manipulate" << std::endl;
            }
            
            // Note: decompose converts to quartenions and breaks current LA implementation of rotations
            LA::vec3 pos, rot, scl;
            ImGuizmo::DecomposeMatrixToComponents((float*)&selectedTrans, (float*)&pos, (float*)&rot, (float*)&scl);
            tc.position = pos;
            tc.rotation = LA::vec3(0.0f) - rot;
            std::cout << "rot " << rot.x << "," << rot.y << "," << rot.z << std::endl;
            tc.scale = scl;
        }

        ImGui::End();
    }
};
