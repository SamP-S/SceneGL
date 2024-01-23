#pragma once

// minimise includes to make it non-backend specific
#include "imgui.h"

//// TODO:
// Rename codebase such that mVar is for member and sVar is static

// abstract class for ImGui windows to consolidate window properties
class IImWindow {
    protected:
        static inline ImGuiWindowFlags _windowFlags = ImGuiWindowFlags_None;

};
