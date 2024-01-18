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

#include "renderer/texture.hpp"
#include "renderer/graphics.hpp"
#include "renderer/input.hpp"
#include "ecs/entity.hpp"
#include "la_extended.h"
using namespace LA;

#include "renderer/light_directional.hpp"
#include "renderer/light_point.hpp"
#include "renderer/mesh_renderer.hpp"
#include "renderer/camera.hpp"
#include "scripting/first_person.hpp"

class Application {
    private:
        // SDL properties
        int gl_major = 4;
        int gl_minor = 6;
        const char* glsl_version = "#version 420 core";
        SDL_GLContext gl_context;
        SDL_Window* window;
        bool isQuit = false;
        int _width = WINDOW_WIDTH;
        int _height = WINDOW_HEIGHT;

        // Engines
        GraphicsEngine Graphics = GraphicsEngine(_width, _height);

        // Application state
        bool show_editor_window = true;
        bool show_render_window = true;
        bool show_stats_window = true;
        bool show_world_window = true;
        bool show_properties_window = true;
        bool show_demo_window = false;
        bool show_firstperson_window = true;
        int texture_load_channel = -1;
        float aspectRatio = 0.0f;
        bool renderer_focused = false;
        ImVec2 render_region_min = ImVec2();
        ImVec2 render_region_max = ImVec2();
        ImVec2 window_pos = ImVec2();
        Entity* entitySelected = NULL;
        int new_entity_count = 0;
        int componentPanelCount = 0;

        std::map<char*, float> arMap = {
            {"None", 0.0f},
            {"16:9", 16.0f / 9.0f},
            {"5:4", 5.0f / 4.0f},
            {"4:3", 4.0f / 3.0f},
            {"1:1", 1.0f}
        };

    public:

        Application() :
        _width(WINDOW_WIDTH),
        _height(WINDOW_HEIGHT) {
            
            Initialise_SDL2(_width, _height);
            Graphics.Initialise();
            Initialise();

            // Load project folder
            entitySelected = nullptr;

            // Main loop
            while (!isQuit)
            {
                HandleEvents();

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
                if (show_world_window) 
                    WorldWindow();
                if (show_properties_window) 
                    PropertiesWindow();
                if (show_firstperson_window)
                    FirstPersonWindow();
                if (show_demo_window) {
                    ImGui::ShowDemoWindow();
                }

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
                    SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
                }

                SDL_GL_SwapWindow(window);
            }
            Shutdown();
            Destroy_SDL2();
        }

        void Initialise_SDL2(int w, int h) {
            // Initialise SDL subsystems
            SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

            // GL 4.6 + GLSL 420
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major) != 0)
                gl_major = 0;
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor) != 0)
                gl_minor = 0;

            // Create window with graphics context
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
            window = SDL_CreateWindow("SceneGL + ImGUI + OpenGL4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, window_flags);
            gl_context = SDL_GL_CreateContext(window);
            SDL_GL_MakeCurrent(window, gl_context);
            SDL_GL_SetSwapInterval(1); // Enable vsync
            SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        void Destroy_SDL2() {
            SDL_GL_DeleteContext(gl_context);
            SDL_DestroyWindow(window);
            SDL_Quit();
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
            ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
            ImGui_ImplOpenGL3_Init(glsl_version);
        }

        void Shutdown() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        void HandleEvents() {
           SDL_Event event;
            // SDL_PollEvent returns 1 while there is an event in the queue
            while (SDL_PollEvent(&event)) {
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
                    if (entitySelected != NULL) {
                        if (ImGui::MenuItem("New Empty")) {
                            // debug
                            std::cout << "Entity: Create new entity" << std::endl;
                            // create empty
                            std::string new_name = "new_" + std::to_string(new_entity_count);
                            
                            Entity* newEntity = new Entity(new_name, entitySelected);
                            entitySelected->AddChild(newEntity);
                            new_entity_count += 1;
                            entitySelected = newEntity;

                            // build components
                            int meshId = resourceMeshes.GetId("empty");
                            MeshRenderer* mr = newEntity->AddComponent<MeshRenderer>();
                            mr->SetMeshId(meshId);
                        }
                        if (ImGui::MenuItem("New Cube")) {
                            // debug
                            std::cout << "Entity: Create new entity" << std::endl;
                            // create entity
                            std::string new_name = "cube_" + std::to_string(new_entity_count);
                            Entity* newEntity = new Entity(new_name, entitySelected);
                            entitySelected->AddChild(newEntity);
                            new_entity_count += 1;
                            entitySelected = newEntity;

                            // build components
                            int meshId = resourceMeshes.GetId("vertex_cube");
                            MeshRenderer* mr = entitySelected->AddComponent<MeshRenderer>();
                            mr->SetMeshId(meshId);
                        }
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Component")) {
                    if (ImGui::MenuItem("Add Camera") && entitySelected != nullptr) {
                        entitySelected->AddComponent<Camera>();
                        std::cout << "DEBUG: Add camera component." << std::endl;
                    }
                    if (ImGui::MenuItem("Add MeshRenderer") && entitySelected != nullptr) {
                        entitySelected->AddComponent<MeshRenderer>();
                        std::cout << "DEBUG: Add MeshRenderer component." << std::endl;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window")) {
                    ImGui::MenuItem("Render Display", NULL, &show_render_window);
                    ImGui::MenuItem("Stats/Performance", NULL, &show_stats_window);
                    ImGui::MenuItem("World Tree", NULL, &show_world_window);
                    ImGui::MenuItem("Viewport Properties", NULL, &show_firstperson_window);
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
            ImGui::Image((ImTextureID)Graphics.frame->GetTexColour(), wSize, ImVec2(0, 1), ImVec2(1, 0));
            // ImGui::GetForegroundDrawList()->AddRect(render_region_min + pos, render_region_max + pos, IM_COL32(255, 255, 0, 255));
            ImGui::End();
        }

        void StatisticsWindow() {
            ImGuiWindowFlags statsWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Statistics Monitor", &show_stats_window, statsWindowFlags);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        void WorldNode(Entity* entitiy) {
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
            if (entitySelected == entitiy)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            if (entitiy->GetNumChildren() == 0)
                node_flags |= ImGuiTreeNodeFlags_Leaf;
            bool node_open = ImGui::TreeNodeEx(entitiy->GetName().c_str(), node_flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                entitySelected = entitiy;
            if (node_open) {
                for (int i = 0; i < entitiy->GetNumChildren(); i++) {
                    WorldNode(entitiy->GetChild(i));
                }
                ImGui::TreePop();
            }   
        }
        
        void WorldWindow() {
            ImGuiWindowFlags worldWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("World Tree", &show_world_window, worldWindowFlags);
            if (Graphics.scene != nullptr) {
                for (auto sceneIt = Graphics.scene->begin(); sceneIt != Graphics.scene->end(); ++sceneIt) {
                    WorldNode(*sceneIt);
                }
            }
            ImGui::End();
        }

        void FirstPersonWindow() {
            ImGuiWindowFlags cameraWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Viewport Properties", &show_firstperson_window, cameraWindowFlags);
            ImGui::PushID(componentPanelCount);
            FirstPersonPanel(Graphics.sceneCam->GetComponent<FirstPersonController>());
            ImGui::PopID();
            ImGui::End();
        }

        void TransformPanel(Transform* t) {
            if (t == nullptr)
                return;
            ImGui::Separator();
            ImGui::Text("Transform:");
            // position
            ImGui::Text("Pos");
            ImGui::SameLine();
            vec3 position = t->GetPosition();
            if (ImGui::InputFloat3("##PosInput", position.m)) {
                t->SetPosition(position);     
            }
            // rotation
            ImGui::Text("Rot");
            ImGui::SameLine();
            vec3 rotation = t->GetRotation();
            if (ImGui::InputFloat3("##RotInput", rotation.m)) {
                t->SetRotation(rotation);     
            }
            // scale
            ImGui::Text("Scl");
            ImGui::SameLine();
            vec3 scale = t->GetScale();
            if (ImGui::InputFloat3("##SclInput", scale.m)) {
                t->SetScale(scale);     
            }
        }

        void MeshRendererPanel(Component* c) {
            if (c == nullptr)
                return;
            MeshRenderer* renderer = dynamic_cast<MeshRenderer*>(c);
            if (renderer == nullptr) {
                return;
            }
            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Mesh Renderer");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                renderer->entity->RemoveComponent<MeshRenderer>(renderer);
            }
            int meshId = renderer->GetMeshId();
            if (ImGui::BeginCombo("Select Mesh", ((meshId == 0) ? "None": resourceMeshes.Get(meshId)->GetName().c_str()))) {
                for (auto it = resourceMeshes.begin(); it != resourceMeshes.end(); it++) {
                    if (ImGui::Selectable(it->second->GetName().c_str())) {
                    renderer->SetMeshId(it->first);
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopID();
            componentPanelCount++;
        }

        void CameraPanel(Component* c) {
            if (c == nullptr)
                return;
            Camera* camera = dynamic_cast<Camera*>(c);
            if (camera == nullptr) {
                return;
            }
            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Camera");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                camera->entity->RemoveComponent<Camera>(camera);
            }
            float fov = camera->GetFov();
            if (ImGui::SliderFloat("FOV", &fov, 45.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoRoundToFormat)) {
                camera->SetFov(fov);
            }
            ImGui::PopID();
            componentPanelCount++;
        }
        
        void DirectionalLightWindow(Component* c) {
            if (c == nullptr)
                return;
            DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(c);
            if (dirLight == nullptr) {
                return;
            }
            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Directional Light");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                dirLight->entity->RemoveComponent<DirectionalLight>(dirLight);
            }
            vec3 colour = dirLight->GetColour();
            if (ImGui::InputFloat3("Colour", colour.m)) {
                dirLight->SetColour(colour);    
            }
            float intensity = dirLight->GetIntensity();
            if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat)) {
                dirLight->SetIntensity(intensity);
            }
            ImGui::PopID();
            componentPanelCount++;
        }

        void PointLightWindow(Component* c) {
            if (c == nullptr)
                return;
            PointLight* pointLight = dynamic_cast<PointLight*>(c);
            if (pointLight == nullptr) {
                return;
            }
            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Point Light");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                pointLight->entity->RemoveComponent<PointLight>(pointLight);
            }
            vec3 colour = pointLight->GetColour();
            if (ImGui::InputFloat3("Colour", colour.m)) {
                pointLight->SetColour(colour);    
            }
            float intensity = pointLight->GetIntensity();
            if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat)) {
                pointLight->SetIntensity(intensity);
            }
            float range = pointLight->GetRange();
            if (ImGui::SliderFloat("Range", &range, 0.01f, 20.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat)) {
                pointLight->SetRange(range);
            }
            ImGui::PopID();
            componentPanelCount++;
        }

        void FirstPersonPanel(Component* c) {
            if (c == nullptr)
                return;
            FirstPersonController* fpc = dynamic_cast<FirstPersonController*>(c);
            if (fpc == nullptr)
                return;
            ImGui::Separator();
            ImGui::Text("First Person Controller");
            // look sensitivity
            float mouseSens = fpc->GetLookSensitivity();
            if (ImGui::SliderFloat("Mouse Sensitivity", &mouseSens, 1.0f, 8.0f, "%.1f", ImGuiSliderFlags_None))
                fpc->SetLookSensitivity(mouseSens);
            // movement speed
            float speed = fpc->GetMovementSpeed();
            if (ImGui::SliderFloat("Movement Speed", &speed, 1.0f, 10.0f, "%.1f", ImGuiSliderFlags_None))
                fpc->SetMovementSpeed(speed);
            componentPanelCount++;
        }

        void PropertiesWindow() {
            ImGuiWindowFlags worldWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Entity Properties", &show_world_window, worldWindowFlags);
            if (entitySelected == NULL) {
                ImGui::Text("Nothing selected");
            } else {
                Entity* ent = entitySelected;
                // entity name
                ImGui::Text("Name:");
                std::string name = ent->GetName();
                char buffer[256];
                std::strncpy(buffer, name.c_str(), sizeof(buffer));
                buffer[sizeof(buffer) - 1] = 0; // Ensure null termination
                if (ImGui::InputText("##NameEntity", buffer, sizeof(buffer))) {
                    ent->SetName(buffer);
                }
                // parent
                Entity* p = ent->GetParent();
                ImGui::Text(("Parent: " + ((p == nullptr) ? "ROOT" : p->GetName())).c_str());
                // components display
                componentPanelCount = 0;
                // transform display
                TransformPanel(ent->transform);
                // non-singlton components
                for (auto* component : ent->_components) {
                    if (component->Type().compare("MeshRenderer") == 0)
                        MeshRendererPanel(component);
                    else if (component->Type().compare("Camera") == 0)
                        CameraPanel(component);
                    else if (component->Type().compare("DirectionalLight") == 0)
                        DirectionalLightWindow(component);
                    else if (component->Type().compare("PointLight") == 0)
                        PointLightWindow(component);
                    else if (component->Type().compare("FirstPersonController") == 0);
                        FirstPersonPanel(component);
                }
                // add component button/drop-down
                if (ImGui::Button("Add Component")) {
                    ImGui::OpenPopup("Add Component");
                }
                if (ImGui::BeginPopup("Add Component")) {
                    if (ImGui::MenuItem("Camera")) {
                        ent->AddComponent<Camera>();
                    } else if (ImGui::MenuItem("Directional Light")) {
                        ent->AddComponent<DirectionalLight>();
                    } else if (ImGui::MenuItem("Point Light")) {
                        ent->AddComponent<PointLight>();
                    } else if (ImGui::MenuItem("Mesh Renderer")) {
                        ent->AddComponent<MeshRenderer>();
                    } else if (ImGui::MenuItem("First Person Controller")) {
                        ent->AddComponent<FirstPersonController>();
                    }
                    ImGui::EndPopup();
                }
                
            }
            ImGui::End();
        }

};