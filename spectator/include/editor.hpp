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

#include "runtime/runtime_controller.hpp"

#include "renderer/context_manager.hpp"
#include "renderer/components.hpp"
#include "renderer/renderer.hpp"
#include "input/input.hpp"

#include "la_extended.h"
using namespace LA;

#include "renderer/editor_camera.hpp"

#include "gui/im_entity.hpp"
#include "gui/im_scene_tree.hpp"
#include "gui/im_statistics.hpp"
#include "gui/im_editor_camera.hpp"
#include "gui/im_viewport.hpp"

class Editor {
    private:
        // OpenGL properties
        OpenGLConfig gl_cfg;

        // SDL
        ContextManager contextManager  = ContextManager();
        bool isQuit = false;

        // Runtime Controller
        RuntimeController runtimeController = RuntimeController();

        // Application state
        // bool show_viewport_window = true;
        // bool show_stats_window = true;
        // bool show_scene_window = true;
        // bool show_entity_window = true;
        // bool show_camera_window = true;
        bool show_demo_window = false;

        int new_entity_count = 0;
        int componentPanelCount = 0;

        AssetManager&  assetManager = AssetManager::Instance();

        // gui windows
        ImViewport imViewport;
        ImStatistics imStatistics;
        ImSceneTree imSceneTree;
        ImEntity imEntity;
        ImEditorCamera imEditorCamera;

    public:
        Editor() {
            // SDL
            contextManager.Initialise(gl_cfg);
            contextManager.AddEventHandler([this](SDL_Event& event) { EventHandler(event); });
            
            Initialise();
            runtimeController.Initialise();

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
                ImGui::PopStyleVar(3);

                ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
                ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
                ImVec2 dockspace_size = ImVec2(0, 0);
                ImGui::DockSpace(dockspace_id, dockspace_size, dockspace_flags);

                MenuBar();
                ImGui::End();

                runtimeController.Tick();

                imViewport.ViewportWindow(runtimeController.renderer->frameBuffer, runtimeController.editorCamera, imSceneTree.entitySelected, imEditorCamera);
                imStatistics.StastisticsWindow(runtimeController.tickTimer);
                imSceneTree.SceneTreeWindow(runtimeController.scene);
                imEntity.EntityWindow(imSceneTree.entitySelected);
                imEditorCamera.EditorCameraWindow(runtimeController.editorCamera);

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
            runtimeController.Shutdown();
            Shutdown();
            contextManager.Destroy();
        }

        void EventHandler(SDL_Event& event) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                isQuit = true;
            }
            
            if (imViewport.isFocuesed) {
                switch (event.type) {
                    case SDL_KEYUP:
                        Input::KeyEvent(event.key.keysym.scancode, KEY_UP);
                        break;
                    case SDL_KEYDOWN:
                        Input::KeyEvent(event.key.keysym.scancode, KEY_DOWN);
                        break;
                    case SDL_MOUSEMOTION:
                        {  
                            ImVec2 sdlCoords = ImVec2(event.motion.x, event.motion.y);
                            ImVec2 viewportCoords = imViewport.GetWindowRelative(sdlCoords);
                            Input::MouseMoved(viewportCoords.x, viewportCoords.y);
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
            // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // allow windows to leave base context
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

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


        void MenuBar() {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New")) {
                        runtimeController.LoadScene("marathon/assets/scenes/Default.json");
                        imSceneTree.entitySelected = Entity();
                    }
                    if (ImGui::MenuItem("Open")) {
                        const char* filepath = tinyfd_openFileDialog("Open Scene", "marathon/assets/scenes/Preset.json", 0, NULL, NULL, 0);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            imSceneTree.entitySelected = Entity();
                            runtimeController.LoadScene(filepath);
                        }
                        
                    }
                    if (ImGui::MenuItem("Save As")) {
                        const char* filepath = tinyfd_saveFileDialog("Save Scene", "marathon/assets/scenes/Test.json", 0, NULL, NULL);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            runtimeController.SaveScene(filepath);
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
                        imSceneTree.entitySelected = runtimeController.scene->CreateEntity();
                    } else if (ImGui::MenuItem("New Cube")) {
                        imSceneTree.entitySelected = runtimeController.scene->CreateEntity();
                        MeshRendererComponent& mrc = imSceneTree.entitySelected.AddComponent<MeshRendererComponent>();
                        mrc.mesh = assetManager.FindAsset<OpenGLMesh>("vertex_cube");
                    } else if (ImGui::MenuItem("New Camera")) {
                        imSceneTree.entitySelected = runtimeController.scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<CameraComponent>();
                    } else if (ImGui::MenuItem("New Directional Light")) {
                        imSceneTree.entitySelected = runtimeController.scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<DirectionalLightComponent>();
                    } else if (ImGui::MenuItem("New Point Light")) {
                        imSceneTree.entitySelected = runtimeController.scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<PointLightComponent>();
                    } else if (ImGui::MenuItem("New Spot Light")) {
                        imSceneTree.entitySelected = runtimeController.scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<SpotLightComponent>();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window")) {
                    ImGui::MenuItem("Render Display", NULL, &imViewport.isOpen);
                    ImGui::MenuItem("Stats/Performance", NULL, &imStatistics.isOpen);
                    ImGui::MenuItem("Scene Tree", NULL, &imSceneTree.isOpen);
                    ImGui::MenuItem("Entity", NULL, &imEntity.isOpen);
                    ImGui::MenuItem("Editor Camera", NULL, &imEditorCamera.isOpen);
                    ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Options")) {
                    if (ImGui::MenuItem("Classic Mode"))
                        ImGui::StyleColorsClassic();
                    else if (ImGui::MenuItem("Light Mode"))
                        ImGui::StyleColorsLight();
                    else if (ImGui::MenuItem("Dark Mode"))
                        ImGui::StyleColorsDark();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
        }

};