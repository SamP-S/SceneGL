#pragma once

#include "gui/im_window.hpp"
#include "core/tick_timer.hpp"

//// TODO:
// add more stats, draw calls?

class ImStatistics : IImWindow {
public:
    static void StastisticsWindow(bool* isOpen, std::shared_ptr<TickTimer> tt) {
        ImGui::Begin("Statistics Monitor", isOpen, _windowFlags);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text(("Tick Count: " + std::to_string(tt->GetTickCount())).c_str());
        ImGui::End();
    }

};
