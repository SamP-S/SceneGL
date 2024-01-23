#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <tinyfiledialogs.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include "ngine/ngine.hpp"
using namespace Ngine;

#include "renderer/context_manager.hpp"
#include "renderer/components.hpp"
#include "renderer/graphics.hpp"
#include "input/input.hpp"

#include "la_extended.h"
using namespace LA;

#include "renderer/editor_camera.hpp"

#include "gui/im_entity.hpp"
#include "gui/im_scene.hpp"

class Editor {
    private:
        // OpenGL properties
        OpenGLConfig gl_cfg;

        // SDL
        ContextManager contextManager  = ContextManager();
        bool isQuit = false;

        // Engines
        GraphicsEngine Graphics = GraphicsEngine(gl_cfg.width, gl_cfg.height);

        // Application state
        bool show_render_window = true;
        bool show_stats_window = true;
        bool show_scene_window = true;
        bool show_entity_window = true;
        bool show_demo_window = false;
        float aspectRatio = 0.0f;
        bool renderer_focused = false;
        ImVec2 render_region_min = ImVec2();
        ImVec2 render_region_max = ImVec2();
        ImVec2 window_pos = ImVec2();
        Entity entitySelected;
        int new_entity_count = 0;
        int componentPanelCount = 0;

        AssetManager&  assetManager = AssetManager::Instance();

        std::map<char*, float> arMap = {
            {"None", 0.0f},
            {"16:9", 16.0f / 9.0f},
            {"5:4", 5.0f / 4.0f},
            {"4:3", 4.0f / 3.0f},
            {"1:1", 1.0f}
        };

    public:
        Editor() {
            // SDL
            contextManager.Initialise(gl_cfg);
            contextManager.AddEventHandler([this](SDL_Event& event) { EventHandler(event); });
            
            Initialise();
            Graphics.Initialise();

            // Main loop
            while (!isQuit)
            {
                contextManager.HandleEvents();
                NewFrame();

                ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();

                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
                window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                
                ImGui::Begin("CoreWindow", NULL, window_flags);
                window_pos = ImGui::GetWindowPos();
                ImGui::PopStyleVar(3);

                ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
                ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
                ImVec2 dockspace_size = ImVec2(0, 0);
                ImGui::DockSpace(dockspace_id, dockspace_size, dockspace_flags);

                MenuBar();
                ImGui::End();

                if (show_render_window)
                    RenderWindow();
                if (show_stats_window) 
                    StatisticsWindow();
                if (show_scene_window) 
                    ImScene::SceneWindow(Graphics.scene, entitySelected, &show_scene_window);
                if (show_entity_window) 
                    ImEntity::EntityWindow(entitySelected, &show_entity_window);
                if (show_demo_window) {
                    ImGui::ShowDemoWindow();
                }

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    contextManager.BackupContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    contextManager.RestoreContext();
                }
                contextManager.SwapFrame();
            }
            Shutdown();
            contextManager.Destroy();
            // Destroy_SDL2();
        }

        void EventHandler(SDL_Event& event) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                isQuit = true;
            }
                        
            if (renderer_focused) {
                switch (event.type) {
                    case SDL_KEYUP:
                        Input::KeyEvent(event.key.keysym.scancode, KEY_UP);
                        break;
                    case SDL_KEYDOWN:
                        Input::KeyEvent(event.key.keysym.scancode, KEY_DOWN);
                        break;
                    case SDL_MOUSEMOTION:
                        {
                            // std::cout << render_region_min.x << ":" << render_region_min.y << std::endl;
                            // std::cout << event.motion.x << "@" << event.motion.y << std::endl;
                            if (event.motion.x >= render_region_min.x && event.motion.y >= render_region_min.y
                                && event.motion.x <= render_region_max.x && event.motion.y <= render_region_max.y) {
                                int x = event.motion.x - render_region_min.x;
                                int y = event.motion.y - render_region_min.y;
                                x = ((x >= 0) ? x : 0);
                                y = ((y >= 0) ? y : 0);
                                x = ((x <= render_region_max.x) ? x : render_region_max.x);
                                y = ((y <= render_region_max.y) ? y : render_region_max.y);
                                Input::MouseMoved(x, y);
                            }
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        Input::MouseButtonEvent(event.button.button, BUTTON_DOWN);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        Input::MouseButtonEvent(event.button.button, BUTTON_UP);
                        break;
                }
            } else {
                Input::ClearStates();
            }
        }
        
        void Initialise() {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(contextManager.window, contextManager.gl_context);
            ImGui_ImplOpenGL3_Init(gl_cfg.glsl);
        }

        void Shutdown() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

        void NewFrame() {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
        }

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

        void MenuBar() {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New")) {
                        Graphics.LoadScene("scene/Default.json");
                    }
                    if (ImGui::MenuItem("Open")) {
                        const char* filepath = tinyfd_openFileDialog("Open Scene", "./scene/Preset.json", 0, NULL, NULL, 0);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            Graphics.LoadScene(filepath);
                        }
                        
                    }
                    if (ImGui::MenuItem("Save As")) {
                        const char* filepath = tinyfd_saveFileDialog("Save Scene", "./scene/Test.json", 0, NULL, NULL);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            Graphics.SaveScene(filepath);
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close")) {
                        isQuit = true;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Entity")) {
                    if (ImGui::MenuItem("New Empty")) {
                        entitySelected = Graphics.scene->CreateEntity();
                    } else if (ImGui::MenuItem("New Cube")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        MeshRendererComponent& mrc = entitySelected.AddComponent<MeshRendererComponent>();
                        mrc.mesh = assetManager.FindAsset<OpenGLMesh>("vertex_cube");
                    } else if (ImGui::MenuItem("New Camera")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<CameraComponent>();
                    } else if (ImGui::MenuItem("New Directional Light")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<DirectionalLightComponent>();
                    } else if (ImGui::MenuItem("New Point Light")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<PointLightComponent>();
                    } else if (ImGui::MenuItem("New Spot Light")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<SpotLightComponent>();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window")) {
                    ImGui::MenuItem("Render Display", NULL, &show_render_window);
                    ImGui::MenuItem("Stats/Performance", NULL, &show_stats_window);
                    ImGui::MenuItem("Scene Tree", NULL, &show_scene_window);
                    ImGui::MenuItem("Entity", NULL, &show_entity_window);
                    ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Aspect Ratio")) {
                    for (auto const& ar : arMap) {
                        if (ImGui::MenuItem(ar.first)) {
                            aspectRatio = ar.second;
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
        }

        void RenderWindow() {
            ImGuiWindowFlags renderWindowFlags = ImGuiWindowFlags_None;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Render Window", &show_render_window, renderWindowFlags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::PopStyleVar(1);
            renderer_focused = ImGui::IsWindowFocused();

            // content size
            ImVec2 pos = ImGui::GetWindowPos();
            pos.x -= window_pos.x;
            pos.y -= window_pos.y;
            // std::cout << "wpos= " << window_pos.x << ":" << window_pos.y << std::endl;
            // std::cout << "pos= " << pos.x << ":" << pos.y << std::endl;
            render_region_min = ImGui::GetWindowContentRegionMin();
            // std::cout << "min= " << render_region_min.x << ":" << render_region_min.y << std::endl;
            render_region_min.x += pos.x;
            render_region_min.y += pos.y;
            // std::cout << "min+= " << render_region_min.x << ":" << render_region_min.y << std::endl;
            render_region_max = ImGui::GetWindowContentRegionMax();
            render_region_max.x += pos.x;
            render_region_max.y += pos.y;
            
            // ImVec2 res = (ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin()) + ImGui::GetWindowPos() - window_pos;
            float wWidth = render_region_max.x - render_region_min.x;
            float wHeight = render_region_max.y - render_region_min.y;
            ImVec2 wSize = AspectRatioLock(ImVec2(wWidth, wHeight), aspectRatio);
            Graphics.Render();
            ImGui::Image((ImTextureID)Graphics.frameBuffer->GetColourAttachment(), wSize, ImVec2(0, 1), ImVec2(1, 0));
            // ImGui::GetForegroundDrawList()->AddRect(render_region_min + pos, render_region_max + pos, IM_COL32(255, 255, 0, 255));
            ImGui::End();
        }

        void StatisticsWindow() {
            ImGuiWindowFlags statsWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Statistics Monitor", &show_stats_window, statsWindowFlags);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

};