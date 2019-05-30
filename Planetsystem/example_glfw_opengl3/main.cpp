#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL // Debug
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp" // Debug
#include "shader.h"
#include "sphere.h"
#include "camera.h"
#include "soil.h"
#include <iostream>

// Legacy glf3.lib support
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Global Variables
int screen_width = 1280;
int screen_height = 720;

ImVec4 background_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

GLint uniform_mvp;

// Planets, Moons and the Sun

// Sun
glm::vec4 sun_color(1.0f, 0.9f, 0.15f, 1.0f);
Sphere sun(1.0f);

// Moons
glm::vec4 moon_color(0.0f, 1.0f, 0.5f, 1.0f);
Sphere earth_moon(0.2f);
Sphere mars_moon(0.2f);
Sphere jupiter_moon(0.2f);

// Earth
glm::vec4 earth_color(0.0f, 1.0f, 0.5f, 1.0f);
Sphere earth(0.5f);

// Mars
glm::vec4 mars_color(1.0f, 0.9f, 0.15f, 1.0f);
Sphere mars(0.7f);

// Jupiter
glm::vec4 jupiter_color(1.0f, 0.9f, 0.15f, 1.0f);
Sphere jupiter(1.0f);

float rot_speed = 1.0f;

bool wireframe_enabled = false;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Menu
bool show_demo_window = true;
bool show_earth = false;
bool show_sun = false;
bool show_earth_moon = false;

glm::mat4 anim = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 mvp = glm::mat4(1.0f);
glm::mat4 translatedmvp = glm::mat4(1.0f);

int InitResources()
{
    // Sun
    sun.SetShader("sphere.v.glsl", "sphere.f.glsl");
    sun.SetColor(sun_color);
    sun.BindBuffers();

    // Earth
    earth.SetShader("sphere.v.glsl", "sphere.f.glsl");
    earth.SetColor(earth_color);
    earth.BindBuffers();

    // Earth Moob
    earth_moon.SetShader("sphere.v.glsl", "sphere.f.glsl");
    earth_moon.SetColor(moon_color);
    earth_moon.BindBuffers();

    // Mars
    mars.SetShader("sphere.v.glsl", "sphere.f.glsl");
    mars.SetColor(mars_color);
    mars.BindBuffers();

    // Mars Moon
    mars_moon.SetShader("sphere.v.glsl", "sphere.f.glsl");
    mars_moon.SetColor(moon_color);
    mars_moon.BindBuffers();

    // Jupiter
    jupiter.SetShader("sphere.v.glsl", "sphere.f.glsl");
    jupiter.SetColor(jupiter_color);
    jupiter.BindBuffers();

    // Jupiter Moon
    jupiter_moon.SetShader("sphere.v.glsl", "sphere.f.glsl");
    jupiter_moon.SetColor(moon_color);
    jupiter_moon.BindBuffers();

    return 1;
}



void Render()
{
    float angle;
    glClearColor(background_color.x, background_color.y, background_color.z, background_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = camera.GetViewMatrix(); // Camera
    projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f); // Projection


    // Sun
    sun.SetColor(sun_color);
    sun.Render(screen_height, screen_width);

    angle = (ImGui::GetTime() / rot_speed) * 50;

    anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), sun.rotation);
    model = glm::translate(glm::mat4(1.0f), sun.position);
    projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);
    mvp = projection * view * model * anim;

    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    // Earth
    earth.SetColor(earth_color);
    earth.Render(screen_height, screen_width);

    anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), earth.rotation);
    model = glm::translate(glm::mat4(1.0f), earth.position);

    mvp = projection * view * model * anim;

    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    //// Earth Moon
    //earth_moon.SetColor(moon_color);
    //earth_moon.Render(screen_height, screen_width);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), earth_moon.rotation);
    //model = glm::translate(glm::mat4(1.0f), earth_moon.position);
    //projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);
    //mvp = projection * view * model * anim;

    //translatedmvp = glm::mat4(1);

    //translatedmvp = mvp * glm::translate(glm::mat4(1.0f), earth.position);

    //glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(translatedmvp));

}

void InitImGui()
{
    const char* shading_elements[]{ "Flat", "Gouraud", "Phong" };
    int selectedItem = 0;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoCollapse);

    ImGui::Combo("Shading", &selectedItem, shading_elements, IM_ARRAYSIZE(shading_elements));
    ImGui::Checkbox("Wireframe", &wireframe_enabled);
    //ImGui::ShowDemoWindow(&show_demo_window); // Debug

    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Text("Position");
    ImGui::SliderFloat3("", glm::value_ptr(camera.position), -20.0f, 20.0f);

    ImGui::Separator();

    ImGui::Checkbox("Earth", &show_earth);

    if (show_earth)
    {
        ImGui::Begin("Earth Settings", &show_earth, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Rotation Speed", &rot_speed, 10.0f, 0.2f);
        ImGui::SliderFloat3("Position", glm::value_ptr(earth.position), -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(earth.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", (float*)& earth_color);

        ImGui::Checkbox("Moon", &show_earth_moon);

        if (show_earth_moon)
        {
            ImGui::Begin("Earth Moon Settings", &show_earth_moon, ImGuiWindowFlags_NoCollapse);
            ImGui::SliderFloat("Rotation Speed", &rot_speed, 10.0f, 0.2f);
            ImGui::SliderFloat3("Position", glm::value_ptr(earth_moon.position), -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotation", glm::value_ptr(earth_moon.rotation), -1.0f, 1.0f);;
            ImGui::ColorEdit4("Color", (float*)& moon_color);
            ImGui::End();
        }

        ImGui::End();
    }

    ImGui::Checkbox("Sun", &show_sun);

    if (show_sun)
    {
        ImGui::Begin("Sun Settings", &show_sun, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Rotation Speed", &rot_speed, 10.0f, 0.2f);
        ImGui::SliderFloat3("Position", glm::value_ptr(sun.position), -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(sun.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", (float*)& sun_color);
        ImGui::End();
    }

    ImGui::Separator();
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Icon
    GLFWimage icons[1];
    icons[0].pixels = SOIL_load_image("icon.png", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
    glfwSetWindowIcon(window, 1, icons);
    SOIL_free_image_data(icons[0].pixels);

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
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        InitImGui();
        Render();
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Wireframe Mode
        if (wireframe_enabled) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

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
