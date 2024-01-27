#pragma once

#include "renderer/context_manager.hpp"
#include "renderer/renderer.hpp"
#include "ngine/ngine.hpp"

class IOperator {
public:
    AssetManager&  assetManager = AssetManager::Instance();
    AssetLoaderManager& loaderManager = AssetLoaderManager::Instance();
    Renderer& renderer = Renderer::Instance();

    virtual void OnInitialise() = 0;
    virtual void OnUpdate(double dt) = 0;
    virtual void OnEvent(SDL_Event& event) = 0;
    virtual void OnShutdown() = 0;
};
