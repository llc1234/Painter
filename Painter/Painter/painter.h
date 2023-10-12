#pragma once
#include <vector>
#include <iostream>
#include <Windows.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"


struct pixel {
    float x = 0;
    float y = 0;

    float r = 1;
    float g = 1;
    float b = 1;
};

struct Vect3 {
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Color3 {
    float r = 0;
    float g = 0;
    float b = 0;
};

class Painter {
	GLFWwindow* window;

	int WindowSizeX = 1100;
	int WindowSizeY = 600;

    const char* WindowTitle = "Painter 0.1.0";

    std::vector<pixel> pixels;

    int pixels_width = 100;
    int pixels_height = 100;

    float CameraX = 0;
    float CameraY = 0;
    float CameraZ = 120;

    Vect3 MousePos;

    // Brush
    bool Brush_Settings_t = false;
    float Brush_width = 0.005;
    float Brush_color[3] = {0, 0, 0};
    
    // screen
    bool Screen_Settings_t = false;
    float background_color[3] = {0.75, 0.75, 0.75};
    

private:

public:
	void Start();
    void Update();
	int SetupWindow();
    void SetViewPort();
    bool WindowShouldClose();

    void SetupImgui();

    void Render();
    void NewFrame();
    void AddNewPixels(int pw, int py);

    void DrawGUI();
    void DrawPixels();

    Vect3 GetMousePos();
    void MoveCamera();

    Vect3 GetMousePos3D(Vect3 mousepos);

    void MainManu();
    void BrushSettings();
    void ScreenSettings();
};

void Painter::SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}

void Painter::SetViewPort() {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    WindowSizeX = w;
    WindowSizeY = h;
}

int Painter::SetupWindow() {
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WindowSizeX, WindowSizeY, WindowTitle, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    SetupImgui();

    AddNewPixels(100, 200);

    MousePos = GetMousePos();

    return 1;
}

void Painter::AddNewPixels(int pw, int py) {
    float x = 0 - (pw / 2);
    float y = 0 + (py / 2);

    for (int i = 0; i < py; i++) {
        for (int j = 0; j < pw; j++) {
            pixel p;
            p.x = x;
            p.y = y;

            // p.r = 0.1 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 0.5);;
            // p.g = 0.1 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 0.5);;
            // p.b = 0.1 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 0.5);;

            pixels.push_back(p);

            x += 1;
        }

        y -= 1;
        x = 0 - (pw / 2);
    }
}

bool Painter::WindowShouldClose() {
    return glfwWindowShouldClose(window);
}

void Painter::NewFrame() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Painter::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void Painter::MainManu() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        ImGui::MenuItem("Save");

        if (ImGui::MenuItem("New")) {
            for (int i = 0; i < pixels.size(); i++) {
                pixels[i].r = 1;
                pixels[i].g = 1;
                pixels[i].b = 1;
            }
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools")) {
        if (ImGui::MenuItem("Brush")) {
            Brush_Settings_t = !Brush_Settings_t;
        }

        if (ImGui::MenuItem("Screen")) {
            Screen_Settings_t = !Screen_Settings_t;
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void Painter::BrushSettings() {
    ImGui::SetNextWindowSize(ImVec2(200, 400));
    ImGui::Begin("Brush");
    ImGui::SliderFloat("width", &Brush_width, 0.005, 0.1);
    ImGui::ColorPicker3("color", Brush_color);
    ImGui::End();
}

void Painter::ScreenSettings() {
    ImGui::SetNextWindowSize(ImVec2(200, 400));
    ImGui::Begin("Screen");
    ImGui::ColorPicker3("background Color", background_color);
    ImGui::SliderFloat("Zoom In", &CameraZ, 1, 150);
    ImGui::End();
}

void Painter::DrawGUI() {
    NewFrame();

    MainManu();

    if (Brush_Settings_t) {
        BrushSettings();
    }

    if (Screen_Settings_t) {
        ScreenSettings();
    }

    Render();
}

Vect3 Painter::GetMousePos() {
    Vect3 v;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    v.x = (float)xpos;
    v.y = (float)ypos;

    return v;
}

void Painter::MoveCamera() {
    Vect3 pos = GetMousePos();

    if ((GetKeyState(VK_RBUTTON) & 0x8000) != 0) {
        CameraX -= (MousePos.x - pos.x) / 1000;
        CameraY += (MousePos.y - pos.y) / 500;
    }
    
    MousePos = pos;
}

Vect3 Painter::GetMousePos3D(Vect3 mousepos) {
    Vect3 new_pos;
    // (mousepos.x / 2 + 0.5) * 1000

    float X = mousepos.x / WindowSizeX;
    float Y = mousepos.y / WindowSizeY;

    new_pos.x = -1.0 + X * (1 - -1);
    new_pos.y = 1.0 + Y * (-1 - 1);

    return new_pos;
}

void Painter::DrawPixels() {
    Vect3 mpos = GetMousePos3D(MousePos);
    
    glBegin(GL_QUADS);

    for (int i = 0; i < pixels.size(); i++) {

        float pox = pixels[i].x / CameraZ;
        float poy = pixels[i].y / CameraZ;

        float pow = 1 / CameraZ;
        float poh = 1 / CameraZ;

        if (CameraX + pox > 1 || CameraX + pox < -1.5) {
            continue;
        }
        
        if (CameraY + poy > 1 || CameraY + poy < -1) {
            continue;
        }

        glColor3f(pixels[i].r, pixels[i].g, pixels[i].b);
        glVertex2f(CameraX + pox, CameraY + poy);
        glVertex2f(CameraX + pox + pow, CameraY + poy);
        glVertex2f(CameraX + pox + pow, CameraY + poy + poh);
        glVertex2f(CameraX + pox, CameraY + poy + poh);

        if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0) {
            float vor = hypot((CameraX + pox) - mpos.x, (CameraY + poy) - mpos.y);

            if (vor < Brush_width) {
                pixels[i].r = Brush_color[0];
                pixels[i].g = Brush_color[1];
                pixels[i].b = Brush_color[2];
            }
        }
    }

    glEnd();
}

void Painter::Update() {
    while (!WindowShouldClose()) {
        SetViewPort();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(background_color[0], background_color[1], background_color[2], 1);

        MoveCamera();
        DrawPixels();
        DrawGUI();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Painter::Start() {
    SetupWindow();
    Update();
}