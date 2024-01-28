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

#include "renderer/components.hpp"
#include "renderer/renderer.hpp"
#include "renderer/editor_camera.hpp"
#include "input/input.hpp"

#include "la_extended.h"
using namespace LA;

#include "runtime/application.hpp"

#include "gui/im_entity.hpp"
#include "gui/im_scene_tree.hpp"
#include "gui/im_statistics.hpp"
#include "gui/im_editor_camera.hpp"
#include "gui/im_viewport.hpp"

//// TODO:
// remove opengl deps

class Editor : public IOperator {
    private:

        // GUI state
        bool show_demo_window = false;

        int new_entity_count = 0;
        int componentPanelCount = 0;

        AssetManager&  assetManager = AssetManager::Instance();
        // should be moved to camera
        
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        std::shared_ptr<EditorCamera> editorCamera = std::make_shared<EditorCamera>();

        // gui windows
        ImViewport imViewport;
        ImStatistics imStatistics;
        ImSceneTree imSceneTree;
        ImEntity imEntity;
        ImEditorCamera imEditorCamera;

    public:
        Editor() = default;
        ~Editor() = default;

        void OnUpdate(double dt) {

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

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

            imViewport.frameBuffer->Bind();

            editorCamera->Update(dt);
            renderer.Clear();
            renderer.RenderSceneByEditorCamera(scene, *editorCamera, DrawMode::FILL);
            renderer.RenderSceneByEditorCamera(scene, *editorCamera, DrawMode::LINES);
            imViewport.frameBuffer->Unbind();

            // fix deps
            imViewport.ViewportWindow(editorCamera, imSceneTree.entitySelected, imEditorCamera);
            imStatistics.StastisticsWindow(dt);
            imSceneTree.SceneTreeWindow(scene);
            imEntity.EntityWindow(imSceneTree.entitySelected);
            imEditorCamera.EditorCameraWindow(editorCamera);

            if (show_demo_window) {
                ImGui::ShowDemoWindow();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void OnEvent(SDL_Event& event) override {
            ImGui_ImplSDL2_ProcessEvent(&event);

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
        
        void OnInitialise() override {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // allow windows to leave base context
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(Application::Get().GetWindow(), Application::Get().GetContext());
            ImGui_ImplOpenGL3_Init(Application::Get().GetOpenGLConfig().glsl);

            renderer.Initialise();
        }

        void OnShutdown() override {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

        // Load Scene from JSON
        void LoadScene(const std::string& filepath) {
            JsonSerializer js = JsonSerializer(scene);
            js.Deserialize(filepath);
        }

        // Save scene to JSON
        void SaveScene(const std::string& filepath) {
            JsonSerializer js = JsonSerializer(scene);
            js.Serialize(filepath);
        }

        void MenuBar() {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New")) {
                        LoadScene("marathon/assets/scenes/Default.json");
                        imSceneTree.entitySelected = Entity();
                    }
                    if (ImGui::MenuItem("Open")) {
                        const char* filepath = tinyfd_openFileDialog("Open Scene", "marathon/assets/scenes/Preset.json", 0, NULL, NULL, 0);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            imSceneTree.entitySelected = Entity();
                            LoadScene(filepath);
                        }
                        
                    }
                    if (ImGui::MenuItem("Save As")) {
                        const char* filepath = tinyfd_saveFileDialog("Save Scene", "marathon/assets/scenes/Test.json", 0, NULL, NULL);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            SaveScene(filepath);
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close")) {
                        Application::Get().Close();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Entity")) {
                    if (ImGui::MenuItem("New Empty")) {
                        imSceneTree.entitySelected = scene->CreateEntity();
                    } else if (ImGui::MenuItem("New Cube")) {
                        imSceneTree.entitySelected = scene->CreateEntity();
                        MeshRendererComponent& mrc = imSceneTree.entitySelected.AddComponent<MeshRendererComponent>();
                        mrc.mesh = assetManager.FindAsset<OpenGLMesh>("vertex_cube");
                    } else if (ImGui::MenuItem("New Camera")) {
                        imSceneTree.entitySelected = scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<CameraComponent>();
                    } else if (ImGui::MenuItem("New Directional Light")) {
                        imSceneTree.entitySelected = scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<DirectionalLightComponent>();
                    } else if (ImGui::MenuItem("New Point Light")) {
                        imSceneTree.entitySelected = scene->CreateEntity();
                        imSceneTree.entitySelected.AddComponent<PointLightComponent>();
                    } else if (ImGui::MenuItem("New Spot Light")) {
                        imSceneTree.entitySelected = scene->CreateEntity();
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