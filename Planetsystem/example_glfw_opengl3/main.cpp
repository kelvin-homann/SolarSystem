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

GLint uniform_model;
GLint uniform_view;
GLint uniform_projection;
GLint uniform_anim;

// Planets, Moons and the Sun
// Sun
glm::vec4 sun_color(1.0f, 0.9f, 0.15f, 1.0f);
Sphere sun(2.0f);
float sun_rot_speed = 1.0f;

// Moons
glm::vec4 moon_color(0.5f, 0.5f, 0.5f, 1.0f);
Sphere earth_moon(0.1f);
float moon_distance_to_earth = 1.0f;
Sphere jupiter_moon(0.1f);
float moon_distance_to_jupiter = 2.0f;

// Earth
glm::vec4 earth_color(0.0f, 0.5f, 1.0f, 1.0f);
Sphere earth(0.5f);
float earth_rot_speed = 3.0f;
float earth_distance_to_sun = 10.f;

// Mars
glm::vec4 mars_color(1.0f, 0.5f, 0.0f, 1.0f);
Sphere mars(0.7f);
float mars_rot_speed = 0.7f;
float mars_distance_to_sun = 5.f;

// Jupiter
glm::vec4 jupiter_color(1.0f, 0.3f, 0.15f, 1.0f);
Sphere jupiter(1.0f);
float jupiter_rot_speed = 0.5f;
float jupiter_distance_to_sun = 15.f;

bool wireframe_enabled = false;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float near_plane = 0.1f;
float far_plane = 10.0f;
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
bool show_mars = false;

bool show_jupiter = false;
bool show_jupiter_moon = false;

glm::mat4 anim = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 mvp = glm::mat4(1.0f);

int InitResources()
{
    camera.position = glm::vec3(0, 0, 14);

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
    projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 50.0f); // Projection
    angle = (ImGui::GetTime() / sun_rot_speed) * 50;

    // Sun
    sun.SetColor(sun_color);
    sun.Render(screen_height, screen_width);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), sun.rotation);
    model = glm::translate(glm::mat4(1.0f), sun.position);

    uniform_model = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "model");
    uniform_view = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "view");
    uniform_projection = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "projection");
    //uniform_anim = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "anim");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(anim));

    model = glm::mat4(1.0f);
    anim = glm::mat4(1.0f);
    angle = (ImGui::GetTime() / earth_rot_speed) * 50;
    
    // Earth
    earth.SetColor(earth_color);
    earth.Render(screen_height, screen_width);

    float earthX = sin(glfwGetTime()) * earth_distance_to_sun;
    float earthY = -0.3f;
    float earthZ = cos(glfwGetTime()) * earth_distance_to_sun;
    glm::vec3 earthPos = glm::vec3(earthX, earthY, earthZ);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), earth.rotation);
    model = glm::translate(glm::mat4(1.0f), sun.position);
    model = glm::translate(model, earthPos);

    uniform_model = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "model");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(anim));

    //std::cout << glm::to_string(earthPos) << std::endl;

    model = glm::mat4(1.0f);
    anim = glm::mat4(1.0f);
    angle = (ImGui::GetTime() / earth_rot_speed) * 50;

    // Earth Moon
    earth_moon.SetColor(moon_color);
    earth_moon.Render(screen_height, screen_width);

    float earth_moonX = sin(glfwGetTime()) * moon_distance_to_earth;
    float earth_moonY = -0.3f;
    float earth_moonZ = cos(glfwGetTime()) * moon_distance_to_earth;
    glm::vec3 earth_moonPos = glm::vec3(earth_moonX, earth_moonY, earth_moonZ);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), earth.rotation);
    model = glm::translate(glm::mat4(1.0f), earthPos);
    model = glm::translate(model, earth_moonPos);

    uniform_model = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "model");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(anim));

    model = glm::mat4(1.0f);
    anim = glm::mat4(1.0f);
    angle = (ImGui::GetTime() / mars_rot_speed) * 50;

    // Mars
    mars.SetColor(mars_color);
    mars.Render(screen_height, screen_width);

    float marsX = sin(glfwGetTime()) * mars_distance_to_sun;
    float marsY = -0.3f;
    float marsZ = cos(glfwGetTime()) * mars_distance_to_sun;
    glm::vec3 marsPos = glm::vec3(marsX, marsY, marsZ);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mars.rotation);
    model = glm::translate(glm::mat4(1.0f), sun.position);
    model = glm::translate(model, marsPos);

    uniform_model = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "model");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(anim));

    model = glm::mat4(1.0f);
    anim = glm::mat4(1.0f);
    angle = (ImGui::GetTime() / earth_rot_speed) * 50;


    // Jupiter
    jupiter.SetColor(jupiter_color);
    jupiter.Render(screen_height, screen_width);

    float jupiterX = sin(glfwGetTime()) * jupiter_distance_to_sun;
    float jupiterY = -0.3f;
    float jupiterZ = cos(glfwGetTime()) * jupiter_distance_to_sun;
    glm::vec3 jupiterPos = glm::vec3(jupiterX, jupiterY, jupiterZ);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), jupiter.rotation);
    model = glm::translate(glm::mat4(1.0f), sun.position);
    model = glm::translate(model, jupiterPos);

    uniform_model = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "model");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(anim));

    // Jupiter Moon
    earth_moon.SetColor(moon_color);
    earth_moon.Render(screen_height, screen_width);

    float jupiter_moonX = sin(glfwGetTime()) * moon_distance_to_earth;
    float jupiter_moonY = -0.3f;
    float jupiter_moonZ = cos(glfwGetTime()) * moon_distance_to_earth;
    glm::vec3 jupiter_moonPos = glm::vec3(jupiter_moonX, jupiter_moonY, jupiter_moonZ);

    //anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), earth.rotation);
    model = glm::translate(glm::mat4(1.0f), jupiterPos);
    model = glm::translate(model, jupiter_moonPos);

    uniform_model = glGetUniformLocation(sun.GetShader().GetShaderProgram(), "model");
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(anim));
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

    //ImGui::ShowDemoWindow(&show_demo_window); // Debug
    ImGui::Text("Shading");
    ImGui::Combo("", &selectedItem, shading_elements, IM_ARRAYSIZE(shading_elements));
    ImGui::Checkbox("Wireframe", &wireframe_enabled);

    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Text("Position");
    ImGui::SliderFloat3("", glm::value_ptr(camera.position), -20.0f, 20.0f);

    ImGui::Separator();

    ImGui::Text("Planets");
    ImGui::Checkbox("Sun", &show_sun); ImGui::SameLine(100);
    ImGui::Checkbox("Earth", &show_earth); 
    ImGui::Checkbox("Mars", &show_mars); ImGui::SameLine(100);
    ImGui::Checkbox("Jupiter", &show_jupiter); 

    if (show_sun)
    {
        ImGui::Begin("Sun Settings", &show_sun, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Rotation Speed", &sun_rot_speed, 10.0f, 0.2f);
        ImGui::SliderFloat3("Position", glm::value_ptr(sun.position), -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(sun.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", (float*)& sun_color);
        ImGui::End();
    }

    if (show_earth)
    {
        ImGui::Begin("Earth Settings", &show_earth, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &earth_distance_to_sun, 1.0f, 20.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(earth.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& earth_color));

        ImGui::Checkbox("Moon", &show_earth_moon);

        if (show_earth_moon)
        {
            ImGui::Begin("Earth Moon Settings", &show_earth_moon, ImGuiWindowFlags_NoCollapse);
            ImGui::SliderFloat("Planet Distance", &moon_distance_to_earth, 1.0f, 20.0f);
            ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& moon_color));
            ImGui::End();
        }

        ImGui::End();
    }

    if (show_mars)
    {
        ImGui::Begin("Mars Settings", &show_mars, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &mars_distance_to_sun, 1.0f, 20.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(mars.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& mars_color));
        ImGui::End();
    }

    if (show_jupiter)
    {
        ImGui::Begin("Jupiter Settings", &show_jupiter, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &jupiter_distance_to_sun, 1.0f, 20.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(jupiter.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& jupiter_color));

        ImGui::Checkbox("Moon", &show_jupiter_moon);

        if (show_jupiter_moon)
        {
            ImGui::Begin("Jupiter Moon Settings", &show_jupiter_moon, ImGuiWindowFlags_NoCollapse);
            ImGui::SliderFloat("Planet Distance", &jupiter_distance_to_sun, 1.0f, 20.0f);
            ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& moon_color));
            ImGui::End();
        }

        ImGui::End();
    }

    ImGui::Separator();
    ImGui::Text("Stats:");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("Background Color");
    ImGui::ColorEdit4("", reinterpret_cast<float*>(& background_color));
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
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
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
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti-Aliasing

    // input
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        float current_frame = glfwGetTime();
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;

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
        glEnable(GL_MULTISAMPLE); // Anti-Aliasing

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup    
    FreeResources(window);

    return 0;
}
