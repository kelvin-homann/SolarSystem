#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <GL/gl3w.h>

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "sphere.h"
#include <iostream>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Global Variables
int screen_width = 1280, screen_height = 720;
float cam_distance = 4.0f;

ImVec4 background_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

// Earth Data
GLint attribute_position, attribute_normals;
GLint uniform_mvp;

glm::vec4 earth_color(0.0f, 1.0f, 0.0f, 0.5f);
Sphere earth(1.0f);

float rot_speed = 1.0f;

//glm::vec3 position = glm::vec3(0.0, 0.0, -4.0f);

float elapsed_time;

int InitResources()
{
    earth.SetShader("sphere.v.glsl", "sphere.f.glsl");
    earth.SetColor(earth_color);
    earth.BindBuffers();

    attribute_position = glGetAttribLocation(earth.GetShader().GetShaderProgram(), "coord3d");
    attribute_normals = glGetAttribLocation(earth.GetShader().GetShaderProgram(), "v_color");
    uniform_mvp = glGetUniformLocation(earth.GetShader().GetShaderProgram(), "mvp");

    return 1;
}

void Render()
{
    glClearColor(background_color.x, background_color.y, background_color.z, background_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    earth.SetColor(earth_color);

    earth.Render(screen_height, screen_width);

    //earth.SetModelViewProjection(screen_height, screen_width);

    float angle = (ImGui::GetTime() / rot_speed) * 50;
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), earth.rotation);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), earth.position);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -cam_distance), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model * anim;

    glm::mat4 translatedmvp = mvp * glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0));

    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void InitImGui()
{
    bool show_demo_window = true;

    const char* shading_elements[]{ "Flat", "Gouraud", "Phong" };
    int selectedItem = 0;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");

    ImGui::Combo("Shading", &selectedItem, shading_elements, IM_ARRAYSIZE(shading_elements));

    ImGui::SliderFloat("Rotation Speed", &rot_speed, 10.0f, 0.2f);

    ImGui::SliderFloat3("Position", glm::value_ptr(earth.position), -10.0f, 10.0f);
    ImGui::SliderFloat3("Rotation", glm::value_ptr(earth.rotation), -1.0f, 1.0f);

    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::ColorEdit4("Color", (float*)& earth_color); 

    ImGui::Text("Stats:");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
    
    // Rendering
    ImGui::Render();
}

void FreeResources(GLFWwindow * window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(earth.GetShader().GetShaderProgram());
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Initialisation of window
    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Planets", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gl3wInit() != 0;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Resource Initialisation
    InitResources();

    // Initialisation of ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        InitImGui();
        Render();
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup    
    FreeResources(window);

    return 0;
}
