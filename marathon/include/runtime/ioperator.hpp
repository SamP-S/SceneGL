#pragma once

#include "renderer/context_manager.hpp"
#include "platform/opengl/opengl_renderer.hpp"
#include "ngine/ngine.hpp"

//// TODO:
// remove opengl deps

class IOperator {
public:
    AssetManager&  assetManager = AssetManager::Instance();
    AssetLoaderManager& loaderManager = AssetLoaderManager::Instance();

    // Modules have capitol first letters
    Renderer& Renderer = OpenGLRenderer::Instance();

    virtual void OnInitialise() = 0;
    virtual void OnUpdate(double dt) = 0;
    virtual void OnEvent(SDL_Event& event) = 0;
    virtual void OnShutdown() = 0;
};
