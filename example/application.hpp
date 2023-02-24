#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <stdlib.h>
#include "texture.hpp"
#include <tinyfiledialogs.h>

#include "graphics.hpp"


class Application {
    private:
        // bool isRunning = true;
        WindowManager windowManager = WindowManager(1280, 720);
        GraphicsEngine Graphics = GraphicsEngine(&windowManager);
        const size_t pixelShaderBufferSize = 8192;
        char pixelShaderBuffer[8192] = 
R"(#version 150 core
// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

uniform vec3    iResolution;
uniform float   iTime;
uniform float   iTimeDelta;
uniform int     iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

out vec4 oColour;

float x = gl_FragCoord.x / iResolution.x;
float y = gl_FragCoord.y / iResolution.y;

void main() {
    oColour = vec4(x, y, 0.5 + 0.5 * cos(iTime), 1.0);
})";

    public:

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

        Application() {
            // initialise GLEW
            // glewExperimental = GL_TRUE;
            // glewInit();

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
            //ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(windowManager.window, windowManager.gl_context);
            ImGui_ImplOpenGL3_Init(windowManager.glsl_version);

            // Application state
            static bool show_editor_window = true;
            static bool show_render_window = true;
            static bool show_stats_window = true;
            static bool show_assets_window = true;
            static bool show_demo_window = false;
            static bool show_file_explorer = false;
            static int texture_load_channel = -1;
            
            static float aspectRatio = 0.0f;

            std::cout << "enter main loop" << std::endl;
            // Main loop
            bool done = false;
            while (!done)
            {
                // Poll and handle events (inputs, window resize, etc.)
                // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
                // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
                // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
                // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
                ;
                SDL_Event event;
                // SDL_PollEvent returns 1 while there is an event in the queue
                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL2_ProcessEvent(&event);
                    switch (event.type) {
                        case SDL_QUIT:
                            windowManager.isQuit = true;
                            break;
                    }
                }


                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();


                //windowManager.PollEvents();
                done = windowManager.isQuit;

                bool opt_fullscreen = true;
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                if (opt_fullscreen) {
                    ImGui::SetNextWindowPos(viewport->WorkPos);
                    ImGui::SetNextWindowSize(viewport->WorkSize);
                    ImGui::SetNextWindowViewport(viewport->ID);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                }

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("DockSpaceWindow", NULL, window_flags);
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
                    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.4f, nullptr, &dock_id_right);
                    ImGuiID dock_id_left_up;
                    ImGuiID dock_id_left_down = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.3f, nullptr, &dock_id_left_up);
                    ImGuiID dock_id_right_up;
                    ImGuiID dock_id_right_down = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.1f, nullptr, &dock_id_right_up);

                    // dock windows into the docking node we made above
                    ImGui::DockBuilderDockWindow("Render Window", dock_id_right_up);
                    ImGui::DockBuilderDockWindow("Pixel Shader Editor", dock_id_left_up);
                    ImGui::DockBuilderDockWindow("Statistics Monitor", dock_id_right_down);
                    ImGui::DockBuilderDockWindow("Asset Manager", dock_id_left_down);
                    ImGui::DockBuilderFinish(dockspace_id);
                    resetDocking = false;
                }
                

                if (ImGui::BeginMenuBar()) {
                    if (ImGui::BeginMenu("File")) {
                        ImGui::MenuItem("New");
                        ImGui::MenuItem("Options");
                        ImGui::Separator();
                        ImGui::MenuItem("Close", NULL, false, true);
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Window")) {
                        ImGui::MenuItem("Shader Editor", NULL, &show_editor_window);
                        ImGui::MenuItem("Render Display", NULL, &show_render_window);
                        ImGui::MenuItem("Asset Manager", NULL, &show_assets_window);
                        ImGui::MenuItem("Stats/Performance", NULL, &show_stats_window);
                        ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Aspect Ratio")) {
                        if (ImGui::MenuItem("None"))
                            aspectRatio = 0.0f;
                        if (ImGui::MenuItem("16:9"))
                            aspectRatio = 16.0f / 9.0f;
                        if (ImGui::MenuItem("5:4"))
                            aspectRatio = 5.0f / 4.0f;
                        if (ImGui::MenuItem("4:3"))
                            aspectRatio = 4.0f / 3.0f;
                        if (ImGui::MenuItem("1:1"))
                            aspectRatio = 1.0f;
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::End();

                // Show editor window
                // ImGuiWindowFlags editorWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
                // if(show_editor_window) { 
                //     ImGui::Begin("Pixel Shader Editor", &show_editor_window, editorWindowFlags);
                //     // Compile button
                //     if (ImGui::Button("Compile")) {
                //         // Graphics.shader.GetShaderFromCharBuffer(pixelShaderBuffer, "FRAGMENT");
                //     }               
                //     ImGui::SameLine();
                //     // ImGui::TextWrapped(Graphics.shader.errorMsg);
                //     // RichText box for writing shader
                //     ImGuiInputTextFlags textAreaFlags = ImGuiInputTextFlags_AllowTabInput;
                //     ImVec2 textAreaSize = ImVec2(-1, -1);
                //     // Instance InputText with no label
                //     ImGui::PushItemWidth(-1);
                //     ImGui::InputTextMultiline("##TextArea Label", pixelShaderBuffer, pixelShaderBufferSize, textAreaSize, textAreaFlags);
                //     ImGui::PopItemWidth();

                //     ImGui::End();
                // }

                // Show render window.
                ImGuiWindowFlags renderWindowFlags = ImGuiWindowFlags_None;
                if (show_render_window) {
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                    ImGui::Begin("Render Window", &show_render_window, renderWindowFlags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                    ImGui::PopStyleVar(1);

                    // content size
                    float wWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
                    float wHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
                    ImVec2 wSize = AspectRatioLock(ImVec2(wWidth, wHeight), aspectRatio);
                    GL_Interface::SetViewport(io.DisplaySize.x, io.DisplaySize.y);
                    GL_Interface::SetClearColour(0.45f, 0.55f, 0.60f, 1.00f);
                    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                    GL_Interface::ClearColourDepth();
                    Graphics.Render();
                    ImGui::Image((ImTextureID)Graphics.texColour, wSize, ImVec2(0, 1), ImVec2(1, 0));
                    ImGui::End();
                }

                // ImGuiWindowFlags assetsWindowFlags = ImGuiWindowFlags_None;
                // if (show_assets_window) {
                //     ImGui::Begin("Asset Manager", &show_assets_window, assetsWindowFlags);
                //     ImGui::Text("Asset Manger:");
                //     // content size
                //     float wWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
                //     float wHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
                //     float wFont = ImGui::GetTextLineHeightWithSpacing();
                //     ImGui::Columns(2, "iChannels");
                //     int channels[4] = {0, 2, 1, 3};
                //     char intStr[16];
                //     for (int i = 0; i < 4; i++) {
                //         ImVec2 wSize = ImVec2(wWidth / 2 - wFont * 2, wHeight / 2 - wFont * 2);
                //         wSize = AspectRatioLock(wSize, Graphics.texBuffer[channels[i]]->GetThumbAspectRatio());
                //         char channelStr[32] = "iChannel";
                //         const char* nameStr = Graphics.texBuffer[channels[i]]->GetNameBuf();
                //         sprintf(intStr, "%d - ", channels[i]);
                //         strcat(channelStr, intStr);
                //         strcat(channelStr, nameStr);
                //         ImGui::Text(channelStr);
                //         if (ImGui::ImageButton((ImTextureID)Graphics.texBuffer[channels[i]]->GetID(), wSize, ImVec2(0, 0), ImVec2(1, 1))) {
                //             show_file_explorer = true;
                //             texture_load_channel = channels[i];
                //         }
                //         if (channels[i] == 2)
                //             ImGui::NextColumn();
                //     }
                    
                //     ImGui::End();
                // }

                const char* imgFilter[7] = { "*.jpg", "*.png", "*.jpeg", "*.tga", "*.bmp", "*.psd", "*.gif" };
                if (show_file_explorer) {
                    char* openFileDialog = tinyfd_openFileDialog(
		                "Open Image File",
		                "",
		                7,
		                imgFilter,
		                NULL,
		                0);
                    std::cout << openFileDialog << std::endl;
                    show_file_explorer = false;
                }

                ImGuiWindowFlags statsWindowFlags = ImGuiWindowFlags_None;
                if (show_stats_window) {
                    ImGui::Begin("Statistics Monitor", &show_stats_window, statsWindowFlags);
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    ImGui::End();
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

};