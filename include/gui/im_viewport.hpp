#pragma once

#include "gui/im_window.hpp"
#include "renderer/frame_buffer.hpp"

//// TODO:
// Generalise viewport to draw any texture, not necessarily a frame buffer
// Should use aspect ratio of texture to ensure it is not stretched
// Find a way to keep original resolution/downsample better

class ImViewport : public IImWindow {
public:
    static inline ImVec2 render_region_min = ImVec2();
    static inline ImVec2 render_region_max = ImVec2();
    static inline bool isFocuesed = false;
    static inline float aspectRatio = 0.0f;

    static ImVec2 AspectRatioLock(const ImVec2 maxSize, float aspectRatio) {
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


    static void ViewportWindow(bool* isOpen, bool& isFocuesed, std::shared_ptr<FrameBuffer> fb) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Render Window", isOpen, _windowFlags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
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
        
        // ImVec2 res = (ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin()) + ImGui::GetWindowPos() - window_pos;
        float wWidth = render_region_max.x - render_region_min.x;
        float wHeight = render_region_max.y - render_region_min.y;
        ImVec2 wSize = AspectRatioLock(ImVec2(wWidth, wHeight), aspectRatio);

        ImGui::Image((ImTextureID)fb->GetColourAttachment(), wSize, ImVec2(0, 1), ImVec2(1, 0));


        ImGui::End();
    }


        // LA::mat4 view = LA::inverse(ec->transform.GetTransform());
        // LA::mat4 proj = ec->GetProjection();
        // LA::mat4 identity = LA::mat4();
        // ImGuizmo::DrawGrid((float*)&view, (float*)&proj, (float*)&identity, 100.f);

        // // ImGuizmo::DrawCubes((float*)&view, (float*)&proj, (float*)&objectMatrix, 1);
        // ImGuizmo::DrawCubes((float*)&view, (float*)&proj, (float*)&identity, 1);

        // // ImGuizmo::Manipulate((float*)&cameraView, (float*)&cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
        // if (ImGuizmo::Manipulate((float*)&view, (float*)&proj, ec->guizmoOp, ec->guizmoCoord, (float*)&identity, NULL, NULL, NULL, NULL)) {
        //     std::cout << "manipulate" << std::endl;
        // }

        // // ImGuizmo::ViewManipulate((float*)&cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
};
