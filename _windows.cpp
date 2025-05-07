// ImGui - standalone example application for Glfw + OpenGL 3, using programmable pipeline

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#pragma comment(lib, "legacy_stdio_definitions.lib")
static GLFWwindow* window;

double mouseX = 0, mouseY = 0, mouseSpeed = 0;
int mouseButtons = 0;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

bool glVesa(int width, int height, int bpp) {
// Setup window
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, 0);
  window = glfwCreateWindow(width, height, "SpritePaint for DOS", NULL, NULL);
  glfwMakeContextCurrent(window);
  gl3wInit();
  
  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(window, true);

// Load Fonts
// (see extra_fonts/README.txt for more details)
//ImGuiIO& io = ImGui::GetIO();
//io.Fonts->AddFontDefault();
//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

// Merge glyphs from multiple fonts into one (e.g. combine default font with another with Chinese glyphs, or add icons)
//ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
//ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 18.0f);
//io.Fonts->AddFontFromFileTTF("../../extra_fonts/fontawesome-webfont.ttf", 18.0f, &icons_config, icons_ranges);


    return true;
}

double glSeconds() {
    static double start = glfwGetTime();
    return glfwGetTime()-start;
}

void glRefresh() {
  glfwSwapBuffers(window);
}

void glDone() {
  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();
}

extern bool askForQuit;

void ImGuiNewFrame() {
    if (glfwWindowShouldClose(window)) {
    askForQuit = true;
    glfwSetWindowShouldClose(window, 0);
    }


    ImGuiIO& io = ImGui::GetIO();
    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();
    ImVec2 mousePos = ImGui::GetMousePos();
    mouseX = mousePos.x;
    mouseY = mousePos.y;
    mouseButtons = (ImGui::IsMouseDown(0) ? 1 : 0) | (ImGui::IsMouseDown(1) ? 2 : 0);
}

void InitImGui(int width, int height) {
  ImGuiStyle& s = ImGui::GetStyle();
  s.WindowRounding = 5.f;
  s.FramePadding = ImVec2(2, 2);
  s.ItemSpacing = ImVec2(3, 3);
  s.WindowPadding = ImVec2(3, 3);
  s.Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 100.0);
  s.Colors[ImGuiCol_TitleBg] = ImVec4(0xff / 255.f, 0, 0x40 / 255.f, 0x20 / 255.f);
  s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0xff / 255.f, 0, 0x40 / 255.f, 0x50 / 255.f);
  s.Colors[ImGuiCol_CloseButton] = ImVec4(0xff / 255.f * 0.15f, 0, 0x40 / 255.f * 0.0f, 0xff / 255.f);
  s.Colors[ImGuiCol_Button] = ImVec4(0xff / 255.f * 0.45f, 0, 0x40 / 255.f * 0.2f, 0xff / 255.f);
}
/*
int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // Load Fonts
    // (see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    // Merge glyphs from multiple fonts into one (e.g. combine default font with another with Chinese glyphs, or add icons)
    //ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    //ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/fontawesome-webfont.ttf", 18.0f, &icons_config, icons_ranges);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        ImGuiIO& io = ImGui::GetIO();
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
*/