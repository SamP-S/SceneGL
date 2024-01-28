#pragma once

#include "gui/im_window.hpp"
#include "core/tick_timer.hpp"

//// TODO:
// add more stats, draw calls?

class ImStatistics : public IImWindow {
public:
    double timeElapsed = 0.0;
    double timeDelta = 0.0;
    int frameCount = 0;

    void StastisticsWindow(double dt) {
        timeElapsed += dt;
        timeDelta = dt;
        frameCount++;

        ImGui::Begin("Statistics Monitor", &isOpen, _windowFlags);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text(("Time Elapsed: " + std::to_string(timeElapsed)).c_str());
        ImGui::Text(("Time Delta: " + std::to_string(timeDelta)).c_str());
        ImGui::Text(("Frame Count: " + std::to_string(frameCount)).c_str());
        ImGui::End();
    }

};
