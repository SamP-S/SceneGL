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
#include "texture.hpp"
#include <tinyfiledialogs.h>

#include "graphics.hpp"
#include "input.hpp"
#include "entity.hpp"


class Application {
    private:
        // bool isRunning = true;
        WindowManager windowManager = WindowManager(1280, 720);
        GraphicsEngine Graphics = GraphicsEngine(&windowManager);

        // Application state
        bool opt_fullscreen = true;
        bool show_editor_window = true;
        bool show_render_window = true;
        bool show_stats_window = true;
        bool show_world_window = true;
        bool show_properties_window = true;
        bool show_demo_window = false;
        bool show_file_explorer = false;
        int texture_load_channel = -1;
        float aspectRatio = 0.0f;
        float prop_pos[3] = {0.0f, 0.0f, 0.0f};
        float prop_rot[3] = {0.0f, 0.0f, 0.0f};
        float prop_scl[3] = {0.0f, 0.0f, 0.0f};
        bool renderer_focused = false;
        ImVec2 render_region_min = ImVec2();
        ImVec2 render_region_max = ImVec2();
        ImVec2 window_pos = ImVec2();

        std::map<char*, float> arMap = {
            {"None", 0.0f},
            {"16:9", 16.0f / 9.0f},
            {"5:4", 5.0f / 4.0f},
            {"4:3", 4.0f / 3.0f},
            {"1:1", 1.0f}
        };

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

    public:

        Application() {

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
            ImGui_ImplSDL2_InitForOpenGL(windowManager.window, windowManager.gl_context);
            ImGui_ImplOpenGL3_Init(windowManager.glsl_version);

            // Main loop
            bool done = false;
            while (!done)
            {
                // Poll and handle events (inputs, window resize, etc.)
                // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
                // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
                // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
                // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
                SDL_Event event;
                // SDL_PollEvent returns 1 while there is an event in the queue
                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL2_ProcessEvent(&event);
                    if (event.type == SDL_QUIT) {
                        windowManager.isQuit = true;
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

                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();

                //windowManager.PollEvents();
                done = windowManager.isQuit;
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                if (opt_fullscreen) {
                    ImGui::SetNextWindowPos(viewport->WorkPos);
                    ImGui::SetNextWindowSize(viewport->WorkSize);
                    ImGui::SetNextWindowViewport(viewport->ID);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
                    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                }

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("CoreWindow", NULL, window_flags);
                window_pos = ImGui::GetWindowPos();
                ImGui::PopStyleVar(3);

                ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
                ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
                ImVec2 dockspace_size = ImVec2(0, 0);
                ImGui::DockSpace(dockspace_id, dockspace_size, dockspace_flags);

                static bool resetDocking = true;
                if (resetDocking) {
                    ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                    ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                    // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                    // window ID to split, direction, fraction (between 0 and 1), 
                    //          the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                    //          out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                    ImGuiID dock_id_right;
                    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dock_id_right);
                    ImGuiID dock_id_left_up;
                    ImGuiID dock_id_left_down = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.3f, nullptr, &dock_id_left_up);
                    ImGuiID dock_id_right_up;
                    ImGuiID dock_id_right_down = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.1f, nullptr, &dock_id_right_up);

                    // dock windows into the docking node we made above
                    ImGui::DockBuilderDockWindow("Render Window", dock_id_right_up);
                    ImGui::DockBuilderDockWindow("World Tree", dock_id_left_up);
                    ImGui::DockBuilderDockWindow("Statistics Monitor", dock_id_right_down);
                    ImGui::DockBuilderDockWindow("Entity Properties", dock_id_left_down);
                    ImGui::DockBuilderFinish(dockspace_id);
                    resetDocking = false;
                }

                MenuBar();
                ImGui::End();

                if (show_render_window) {
                    // std::cout << ImGui::GetWindowPos().x << "#" << ImGui::GetWindowPos().y << std::endl;
                    RenderWindow();
                }
                if (show_stats_window) {
                    StatisticsWindow();
                }
                if (show_world_window) {
                    WorldWindow();
                }
                if (show_properties_window) {
                    PropertiesWindow();
                }
                if (show_demo_window) {
                    ImGui::ShowDemoWindow();
                }

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
                    SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
                }

                SDL_GL_SwapWindow(windowManager.window);
                // windowManager.SwapBuffers();
            }

            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

        void MenuBar() {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    ImGui::MenuItem("New");
                    ImGui::MenuItem("Options");
                    ImGui::Separator();
                    ImGui::MenuItem("Close", NULL, false, true);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window")) {
                    ImGui::MenuItem("Render Display", NULL, &show_render_window);
                    ImGui::MenuItem("Stats/Performance", NULL, &show_stats_window);
                    ImGui::MenuItem("World Tree", NULL, &show_world_window);
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
            
            float wWidth = render_region_max.x - render_region_min.x;
            float wHeight = render_region_max.y - render_region_min.y;
            ImVec2 wSize = AspectRatioLock(ImVec2(wWidth, wHeight), aspectRatio);
            Graphics.Render();
            ImGui::Image((ImTextureID)Graphics.texColour, wSize, ImVec2(0, 1), ImVec2(1, 0));
            // ImGui::GetForegroundDrawList()->AddRect(render_region_min + pos, render_region_max + pos, IM_COL32(255, 255, 0, 255));
            ImGui::End();
        }

        void StatisticsWindow() {
            ImGuiWindowFlags statsWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Statistics Monitor", &show_stats_window, statsWindowFlags);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        void WorldNode(EntityId entId) {
            Entity* ent = resourceEntities.Get(entId);
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (Graphics.worldSelected == entId)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            if (ent->GetNumChildren() == 0)
                node_flags |= ImGuiTreeNodeFlags_Leaf;
            bool node_open = ImGui::TreeNodeEx(ent->GetName().c_str(), node_flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                Graphics.worldSelected = entId;
            if (node_open) {
                for (int i = 0; i < ent->GetNumChildren(); i++) {
                    WorldNode(ent->GetChild(i));
                }
                ImGui::TreePop();
            }   
        }
        
        void WorldWindow() {
            ImGuiWindowFlags worldWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("World Tree", &show_world_window, worldWindowFlags);
            for (int i = 0; i < Graphics.world.size(); i++) {
                WorldNode(Graphics.world.at(i));
            }
            ImGui::End();
        }

        void PropertiesWindow() {
            ImGuiWindowFlags worldWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Entity Properties", &show_world_window, worldWindowFlags);
            if (Graphics.worldSelected == -1) {
                ImGui::Text("Nothing selected");
            } else {
                Entity* ent = resourceEntities.Get(Graphics.worldSelected);
                ImGui::Text(ent->GetName().c_str());
                ImGui::Text("Transform:");
                // ImGui::Text("Position:");
                // position
                memcpy(&prop_pos, &ent->trans.GetPosition()[0], sizeof(float) * 3);
                ImGui::Text("Pos");
                ImGui::SameLine();
                if (ImGui::InputFloat3("##PosInput", prop_pos)) {
                   ent->trans.SetPosition(vec3{prop_pos[0], prop_pos[1], prop_pos[2]});     
                }
                // rotation
                memcpy(&prop_rot, &ent->trans.GetRotation()[0], sizeof(float) * 3);
                ImGui::Text("Rot");
                ImGui::SameLine();
                if (ImGui::InputFloat3("##RotInput", prop_rot)) {
                   ent->trans.SetRotation(vec3{prop_rot[0], prop_rot[1], prop_rot[2]});     
                }
                // scale
                memcpy(&prop_scl, &ent->trans.GetScale()[0], sizeof(float) * 3);
                ImGui::Text("Scl");
                ImGui::SameLine();
                if (ImGui::InputFloat3("##SclInput", prop_scl)) {
                   ent->trans.SetScale(vec3{prop_scl[0], prop_scl[1], prop_scl[2]});     
                }
                int meshId = ent->GetMesh();
                if (ImGui::BeginCombo("Select Mesh", ((meshId < 0) ? "None": resourceMeshes.Get(meshId)->GetName().c_str()))) {
                    for (auto it = resourceMeshes.Begin(); it != resourceMeshes.End(); it++) {
                        if (ImGui::Selectable(it->second->GetName().c_str())) {
                           ent->SetMesh(it->first);
                        }
                    }
                    ImGui::EndCombo();
                }

            }
            ImGui::End();
        }

};