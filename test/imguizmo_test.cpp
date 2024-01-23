// https://github.com/CedricGuillemet/ImGuizmo
// v 1.89 WIP
//
// The MIT License(MIT)
//
// Copyright(c) 2021 Cedric Guillemet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include "la_extended.h"
#include "ImGuizmo.h"

#include <vector>
#include <algorithm>


namespace ImApp {
   struct Config
   {
      Config() : mWidth(1280), mHeight(720), mFullscreen(false) {}
      int mWidth;
      int mHeight;
      bool mFullscreen;
   };

   class ImApp {
   public:
      Config mConfig;
      bool isQuit = false;
      SDL_GLContext gl_context;
      SDL_Window* window;
      const char* glsl_version = "#version 460 core";

      bool Done() {
         return isQuit;
      }

      void Init(Config cfg) {
         mConfig = cfg;
         SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

         SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
         window = SDL_CreateWindow(
            "SceneGL + ImGUI + OpenGL4", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            cfg.mWidth, cfg.mHeight, 
            window_flags
         );
         gl_context = SDL_GL_CreateContext(window);

         SDL_GL_MakeCurrent(window, gl_context);
         SDL_GL_SetSwapInterval(1); // Enable vsync

         ImGui::CreateContext();
         ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
         ImGui_ImplOpenGL3_Init(glsl_version);
      }

      void NewFrame() {
         SDL_Event event;
         while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
               isQuit = true;
            }
         }

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplSDL2_NewFrame();
         ImGui::NewFrame();
      }

      void EndFrame() {
         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         SDL_GL_SwapWindow(window);
      }

      void Finish() {
         // shutdown imgui
         ImGui_ImplOpenGL3_Shutdown();
         ImGui_ImplSDL2_Shutdown();
         ImGui::DestroyContext();

         // shutdown sdl2
         SDL_GL_DeleteContext(gl_context);
         SDL_DestroyWindow(window);
         SDL_Quit();
      }
   };
}


bool useWindow = true;
float camDistance = 8.f;
static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

LA::mat4 objectMatrix = 
   LA::mat4({
      { 1.f, 0.f, 0.f, 0.f},
      { 0.f, 1.f, 0.f, 0.f},
      { 0.f, 0.f, 1.f, 0.f},
      { 0.f, 0.f, 0.f, 1.f}
   });

static const LA::mat4 identityMatrix = LA::mat4();


void EditTransform(LA::mat4& cameraView, LA::mat4& cameraProjection, LA::mat4& matrix, bool editTransformDecomposition)
{
   static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
   static bool useSnap = false;
   static float snap[3] = { 1.f, 1.f, 1.f };
   static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
   static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
   static bool boundSizing = false;
   static bool boundSizingSnap = false;

   if (editTransformDecomposition)
   {
      if (ImGui::IsKeyPressed(ImGuiKey_T))
         mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
      if (ImGui::IsKeyPressed(ImGuiKey_E))
         mCurrentGizmoOperation = ImGuizmo::ROTATE;
      if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
         mCurrentGizmoOperation = ImGuizmo::SCALE;
      if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
         mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
      ImGui::SameLine();
      if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
         mCurrentGizmoOperation = ImGuizmo::ROTATE;
      ImGui::SameLine();
      if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
         mCurrentGizmoOperation = ImGuizmo::SCALE;
      ImGui::SameLine();
      if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
         mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
      float matrixTranslation[3], matrixRotation[3], matrixScale[3];
      ImGuizmo::DecomposeMatrixToComponents((float*)&matrix, matrixTranslation, matrixRotation, matrixScale);
      ImGui::InputFloat3("Tr", matrixTranslation);
      ImGui::InputFloat3("Rt", matrixRotation);
      ImGui::InputFloat3("Sc", matrixScale);
      ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, (float*)&matrix);

      if (mCurrentGizmoOperation != ImGuizmo::SCALE)
      {
         if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
         ImGui::SameLine();
         if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
      }
      if (ImGui::IsKeyPressed(ImGuiKey_S))
         useSnap = !useSnap;
      ImGui::Checkbox("##UseSnap", &useSnap);
      ImGui::SameLine();

      switch (mCurrentGizmoOperation)
      {
      case ImGuizmo::TRANSLATE:
         ImGui::InputFloat3("Snap", &snap[0]);
         break;
      case ImGuizmo::ROTATE:
         ImGui::InputFloat("Angle Snap", &snap[0]);
         break;
      case ImGuizmo::SCALE:
         ImGui::InputFloat("Scale Snap", &snap[0]);
         break;
      }
      ImGui::Checkbox("Bound Sizing", &boundSizing);
      if (boundSizing)
      {
         ImGui::PushID(3);
         ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
         ImGui::SameLine();
         ImGui::InputFloat3("Snap", boundsSnap);
         ImGui::PopID();
      }
   }

   ImGuiIO& io = ImGui::GetIO();
   float viewManipulateRight = io.DisplaySize.x;
   float viewManipulateTop = 0;
   static ImGuiWindowFlags gizmoWindowFlags = 0;
   if (useWindow)
   {
      ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
      ImGui::SetNextWindowPos(ImVec2(400,20), ImGuiCond_Appearing);
      ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
      ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
      ImGuizmo::SetDrawlist();
      float windowWidth = (float)ImGui::GetWindowWidth();
      float windowHeight = (float)ImGui::GetWindowHeight();
      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
      viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
      viewManipulateTop = ImGui::GetWindowPos().y;
      ImGuiWindow* window = ImGui::GetCurrentWindow();
      gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
   } else {
      ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
   }

   ImGuizmo::DrawGrid((float*)&cameraView, (float*)&cameraProjection, (float*)&identityMatrix, 100.f);
   ImGuizmo::DrawCubes((float*)&cameraView, (float*)&cameraProjection, (float*)&objectMatrix, 1);
   ImGuizmo::Manipulate((float*)&cameraView, (float*)&cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

   ImGuizmo::ViewManipulate((float*)&cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

   if (useWindow)
   {
      ImGui::End();
      ImGui::PopStyleColor(1);
   }
}

int main(int, char**)
{
   ImApp::ImApp imApp;

   ImApp::Config config;
   config.mWidth = 1280;
   config.mHeight = 720;
   config.mFullscreen = true;
   imApp.Init(config);

   int lastUsing = 0;

   LA::mat4 cameraView = LA::mat4();
   LA::mat4 cameraProjection = LA::mat4();

   // Camera projection
   bool isPerspective = true;
   float fov = 27.f;
   float viewWidth = 10.f; // for orthographic
   float camYAngle = 165.f / 180.f * 3.14159f;
   float camXAngle = 32.f / 180.f * 3.14159f;

   bool firstFrame = true;

   // Main loop
   while (!imApp.Done())
   {
      imApp.NewFrame();

      ImGuiIO& io = ImGui::GetIO();
      if (isPerspective)
      {
         cameraProjection = LA::Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f);
      }
      else
      {
         float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
         cameraProjection = LA::Orthographic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f);
      }
      ImGuizmo::SetOrthographic(!isPerspective);
      ImGuizmo::BeginFrame();

      ImGui::SetNextWindowPos(ImVec2(1024, 100), ImGuiCond_Appearing);
      ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Appearing);

      // create a window and insert the inspector
      ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
      ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);
      ImGui::Begin("Editor");
      if (ImGui::RadioButton("Full view", !useWindow)) useWindow = false;
      ImGui::SameLine();
      if (ImGui::RadioButton("Window", useWindow)) useWindow = true;

      ImGui::Text("Camera");
      bool viewDirty = false;
      if (ImGui::RadioButton("Perspective", isPerspective)) isPerspective = true;
      ImGui::SameLine();
      if (ImGui::RadioButton("Orthographic", !isPerspective)) isPerspective = false;
      if (isPerspective)
      {
         ImGui::SliderFloat("Fov", &fov, 20.f, 110.f);
      }
      else
      {
         ImGui::SliderFloat("Ortho width", &viewWidth, 1, 20);
      }
      viewDirty |= ImGui::SliderFloat("Distance", &camDistance, 1.f, 10.f);

      if (viewDirty || firstFrame)
      {
         LA::vec3 eye = LA::vec3({ 
            cosf(camYAngle) * cosf(camXAngle) * camDistance,
            sinf(camXAngle) * camDistance,
            sinf(camYAngle) * cosf(camXAngle) * camDistance
         });
         LA::vec3 at = LA::vec3({ 0.f, 0.f, 0.f });
         LA::vec3 up = LA::vec3({ 0.f, 1.f, 0.f });
         cameraView = LA::LookAt(eye, at, up);
         firstFrame = false;
      }

      ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
      if (ImGuizmo::IsUsing())
      {
         ImGui::Text("Using gizmo");
      }
      else
      {
         ImGui::Text(ImGuizmo::IsOver()?"Over gizmo":"");
         ImGui::SameLine();
         ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
         ImGui::SameLine();
         ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
         ImGui::SameLine();
         ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
      }
      ImGui::Separator();
      ImGuizmo::SetID(0);

      EditTransform(cameraView, cameraProjection, objectMatrix, true);
      
      ImGui::End();
      
      // render everything
      glClearColor(0.45f, 0.4f, 0.4f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT);
      imApp.EndFrame();
   }

   imApp.Finish();

   return 0;
}
