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
#include "light.h"
#include <iostream>

// Legacy glf3.lib support
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

int screen_width = 1280;
int screen_height = 720;

ImVec4 background_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

// Uniform Locations
GLint uniform_model;
GLint uniform_view;
GLint uniform_projection;

// Sun
glm::vec4 sun_color(1.0f, 0.9f, 0.15f, 1.0f);
Sphere sun(10.0f);
float sun_rot_speed = 1.0f;

// Moons
glm::vec4 moon_color(0.5f, 0.5f, 0.5f, 1.0f);
Sphere earth_moon(1.0f);
float earth_moon_rot_speed = 12.0f;
float moon_distance_to_earth = 15.f;
Sphere mars_moon(1.0f);
float moon_distance_to_mars = 10.0f;

// Earth
Sphere earth(3.f);
float earth_rot_speed = 1.0f;
float earth_distance_to_sun = 70.f;

// Mars
glm::vec4 mars_color(1.0f, 0.5f, 0.0f, 1.0f);
Sphere mars(2.5f);
float mars_rot_speed = 0.7f;
float mars_distance_to_sun = 35.f;

// Venus
glm::vec4 venus_color(1.0f, 0.3f, 0.15f, 1.0f);
Sphere venus(3.0f);
float venus_rot_speed = 0.5f;
float venus_distance_to_sun = 55.f;

bool wireframe_enabled = false;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 cameraOffset(0, 20, 0);
float near_plane = 0.1f;
float far_plane = 1000.0f;
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
double elapsedTime = 0.0;
double elapsedTimeScaled = 0.0;
double lastElapsedTime = 0.0;
float timeScale = 1.f;

// Menu
bool show_demo_window = true;
bool show_earth = false;
bool show_sun = false;
bool show_earth_moon = false;
bool show_mars = false;
bool show_mars_moon = false;
bool show_venus = false;

glm::mat4 anim = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 mvp = glm::mat4(1.0f);

// Light
Light light = Light();

int InitResources()
{
    // Light
    light.position = sun.position;
    light.color = glm::vec3(1.0f, 1.0f, 0.8f);
    light.diffuse = glm::vec3(1.0f, 1.0f, 0.8f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Camera
    camera.position = glm::vec3(-140, 140, 10);
    camera.SetPitch(-45.f);
    camera.Yaw = 0.f;

    // Sun
    sun.SetShader("material.v.glsl", "material.f.glsl");
    sun.BindBuffers();
    sun.material.ambient = glm::vec3(1.0f, 1.0f, 0.8f);
    sun.material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.material.shininess = 32.0f;

    // Earth
    earth.SetShader("material.v.glsl", "material.f.glsl");
    earth.BindBuffers();
    earth.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    earth.material.diffuse = glm::vec3(0.0f, 0.5f, 1.0f);
    earth.material.specular = glm::vec3(0.0f, 0.5f, 1.0f); 
    earth.material.shininess = 32.0f;

    // Earth Moob
    earth_moon.SetShader("phong.v.glsl", "phong.f.glsl");
    earth_moon.BindBuffers();

    // Mars
    mars.SetShader("phong.v.glsl", "phong.f.glsl");
    mars.BindBuffers();

    // Jupiter
    venus.SetShader("phong.v.glsl", "phong.f.glsl");
    venus.BindBuffers();

    // Jupiter Moon
    mars_moon.SetShader("phong.v.glsl", "phong.f.glsl");
    mars_moon.BindBuffers();

    return 1;
}

void Render()
{
    // Timing
    lastElapsedTime = elapsedTime;
    elapsedTime = glfwGetTime();
    float dt = elapsedTime - lastElapsedTime;
    float dtScaled = dt * timeScale;
    elapsedTimeScaled += dtScaled;

    float angle;
    glClearColor(background_color.x, background_color.y, background_color.z, background_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.updateCameraVectors();

    glm::vec3 earthPosRel = glm::vec3(sin(elapsedTimeScaled * earth_rot_speed), 0.f, cos(elapsedTimeScaled * earth_rot_speed)) * earth_distance_to_sun;
    glm::vec3 earthPosAbs = sun.position + earthPosRel;

    glm::vec3 moonPosRel = glm::vec3(sin(elapsedTimeScaled * earth_moon_rot_speed), 0.f, cos(elapsedTimeScaled * earth_moon_rot_speed)) * moon_distance_to_earth;
    glm::vec3 moonPosAbs = earthPosAbs + moonPosRel;

    glm::vec3 marsPosRel = glm::vec3(sin(elapsedTimeScaled * mars_rot_speed), 0.f, cos(elapsedTimeScaled * mars_rot_speed)) * mars_distance_to_sun;
    glm::vec3 marsPosAbs = sun.position + marsPosRel;

    glm::vec3 marsmoonPosRel = glm::vec3(sin(elapsedTimeScaled * 3.0f), 0.f, cos(elapsedTimeScaled * 3.0f)) * moon_distance_to_mars;
    glm::vec3 marsmoonPosAbs = marsPosAbs + marsmoonPosRel;

    glm::vec3 venusPosRel = glm::vec3(sin(elapsedTimeScaled * venus_rot_speed), 0.f, cos(elapsedTimeScaled * venus_rot_speed)) * venus_distance_to_sun;
    glm::vec3 venusPosAbs = sun.position + venusPosRel;

    glm::mat4 view = camera.GetViewMatrix(); // Camera
    projection = glm::perspective(glm::radians(camera.Zoom), 1.0f * screen_width / screen_height, near_plane, far_plane); // Projection

    // Sun
    angle = (elapsedTimeScaled / sun_rot_speed) * 50;

    sun.BindShader();
    model = glm::translate(glm::mat4(1.0f), sun.position); // Settings Sun Position

    sun.GetShader().setVec3("light.ambient", light.color);
    sun.GetShader().setVec3("light.diffuse", light.diffuse);
    sun.GetShader().setVec3("light.specular", light.specular);

    // material properties
    sun.GetShader().setVec3("material.ambient", sun.material.ambient);
    sun.GetShader().setVec3("material.diffuse", sun.material.diffuse);
    sun.GetShader().setVec3("material.specular", sun.material.specular);
    sun.GetShader().setFloat("material.shininess", sun.material.shininess);

    sun.GetShader().setVec3("light.position", light.position);
    sun.GetShader().setVec3("viewPos", camera.position);
    sun.GetShader().setMat4("projection", projection);
    sun.GetShader().setMat4("view", view);
    sun.GetShader().setMat4("model", model);
    sun.Render(screen_height, screen_width);
    
    // Earth
    angle = elapsedTimeScaled / earth_rot_speed;

    earth.BindShader();
    model = glm::translate(glm::mat4(1.0f), earthPosAbs);
    model = glm::rotate(model, angle, earth.rotation);

    earth.GetShader().setVec3("light.ambient", light.color);
    earth.GetShader().setVec3("light.diffuse", light.diffuse);
    earth.GetShader().setVec3("light.specular", light.specular);

    // material properties
    earth.GetShader().setVec3("material.ambient", earth.material.ambient);
    earth.GetShader().setVec3("material.diffuse", earth.material.diffuse);
    earth.GetShader().setVec3("material.specular", earth.material.specular);
    earth.GetShader().setFloat("material.shininess", earth.material.shininess);

    earth.GetShader().setVec3("light.position", light.position);
    earth.GetShader().setVec3("viewPos", camera.position);
    earth.GetShader().setMat4("projection", projection);
    earth.GetShader().setMat4("view", view);
    earth.GetShader().setMat4("model", model);
    earth.Render(screen_height, screen_width);

    // Earth Moon
    earth_moon.BindShader();
    model = glm::translate(glm::mat4(1.0f), moonPosAbs);
    earth_moon.GetShader().setVec3("objectColor", moon_color);
    earth_moon.GetShader().setVec3("lightColor", light.color);
    earth_moon.GetShader().setVec3("lightPos", light.position);
    earth_moon.GetShader().setVec3("viewPos", camera.position);
    earth_moon.GetShader().setMat4("projection", projection);
    earth_moon.GetShader().setMat4("view", view);
    earth_moon.GetShader().setMat4("model", model);
    earth_moon.Render(screen_height, screen_width);

    // Mars
    angle = elapsedTimeScaled / mars_rot_speed;

    mars.BindShader();
    model = glm::translate(glm::mat4(1.0f), marsPosAbs);
    model = glm::rotate(model, angle, mars.rotation);

    mars.GetShader().setVec3("objectColor", mars_color);
    mars.GetShader().setVec3("lightColor", light.color);
    mars.GetShader().setVec3("lightPos", light.position);
    mars.GetShader().setVec3("viewPos", camera.position);
    mars.GetShader().setMat4("projection", projection);
    mars.GetShader().setMat4("view", view);
    mars.GetShader().setMat4("model", model);

    mars.SetColor(mars_color);
    mars.Render(screen_height, screen_width);

    // Mars Moon
    mars_moon.BindShader();
    model = glm::translate(glm::mat4(1.0f), marsmoonPosAbs);

    mars_moon.GetShader().setVec3("objectColor", moon_color);
    mars_moon.GetShader().setVec3("lightColor", light.color);
    mars_moon.GetShader().setVec3("lightPos", light.position);
    mars_moon.GetShader().setVec3("viewPos", camera.position);
    mars_moon.GetShader().setMat4("projection", projection);
    mars_moon.GetShader().setMat4("view", view);
    mars_moon.GetShader().setMat4("model", model);

    mars_moon.SetColor(moon_color);
    mars_moon.Render(screen_height, screen_width);

    // Venus
    angle = elapsedTimeScaled / mars_rot_speed;

    venus.BindShader();
    model = glm::translate(glm::mat4(1.0f), venusPosAbs);
    model = glm::rotate(model, angle, venus.rotation);

    venus.GetShader().setVec3("objectColor", venus_color);
    venus.GetShader().setVec3("lightColor", light.color);
    venus.GetShader().setVec3("lightPos", light.position);
    venus.GetShader().setVec3("viewPos", camera.position);
    venus.GetShader().setMat4("projection", projection);
    venus.GetShader().setMat4("view", view);
    venus.GetShader().setMat4("model", model);

    venus.SetColor(venus_color);
    venus.Render(screen_height, screen_width);
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
    ImGui::Text("Time");
    ImGui::SliderFloat("Time", &timeScale, 0.f, 10.f, "%.2f");
    ImGui::ColorEdit3("Light Color", reinterpret_cast<float*>(&light.color));
    ImGui::SliderFloat3("Light Position", glm::value_ptr(light.position), -50.0f, 50.0f);
    ImGui::SliderFloat3("Light Diffuse", glm::value_ptr(light.diffuse), -50.0f, 50.0f);
    ImGui::SliderFloat3("Light Specular", glm::value_ptr(light.specular), -50.0f, 50.0f);

    ImGui::Text("Camera");
    ImGui::InputFloat3("Camera Position", glm::value_ptr(camera.position), 2);
    ImGui::SliderFloat("Camera Pitch", &camera.Pitch, -89.f, 89.f, "%.2f");
    ImGui::SliderFloat("Camera Yaw", &camera.Yaw, -89.f, 89.f, "%.2f");

    ImGui::Separator();

    ImGui::Text("Planets");
    ImGui::Checkbox("Sun", &show_sun); ImGui::SameLine(100);
    ImGui::Checkbox("Earth", &show_earth); 
    ImGui::Checkbox("Mars", &show_mars); ImGui::SameLine(100);
    ImGui::Checkbox("Venus", &show_venus); 

    if (show_sun)
    {
        ImGui::Begin("Sun Settings", &show_sun, ImGuiWindowFlags_NoCollapse);
        ImGui::InputFloat3("Position", glm::value_ptr(sun.position), 2);
        ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& sun_color));
        ImGui::End();
    }

    if (show_earth)
    {
        ImGui::Begin("Earth Settings", &show_earth, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &earth_distance_to_sun, 1.0f, 20.0f);
        ImGui::SliderFloat("Speed", &earth_rot_speed, 1.0f, 5.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(earth.rotation), -1.0f, 1.0f);;

        ImGui::Separator();

        // Material
        ImGui::Text("Material");
        ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(& earth.material.ambient));
        ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(& earth.material.diffuse));
        ImGui::SliderFloat3("Specular", glm::value_ptr(earth.material.specular), 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &earth.material.shininess, 1.0f, 50.0f);

        ImGui::Separator();

        ImGui::Checkbox("Moon", &show_earth_moon);

        if (show_earth_moon)
        {
            ImGui::Begin("Earth Moon Settings", &show_earth_moon, ImGuiWindowFlags_NoCollapse);
            ImGui::SliderFloat("Planet Distance", &moon_distance_to_earth, 1.0f, 20.0f);
            ImGui::SliderFloat("Rotation Speed", &earth_moon_rot_speed, 1.0f, 20.0f);
            ImGui::ColorEdit4("Color", reinterpret_cast<float*>(& moon_color));
            ImGui::End();
        }
        ImGui::End();
    }

    if (show_mars)
    {
        ImGui::Begin("Mars Settings", &show_mars, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &mars_distance_to_sun, 1.0f, 20.0f);
        ImGui::SliderFloat("Speed", &mars_rot_speed, 1.0f, 5.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(mars.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&mars_color));

        ImGui::Checkbox("Moon", &show_mars_moon);

        if (show_mars_moon)
        {
            ImGui::Begin("Mars Moon Settings", &show_mars_moon, ImGuiWindowFlags_NoCollapse);
            ImGui::SliderFloat("Planet Distance", &moon_distance_to_mars, 1.0f, 20.0f);
            ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&moon_color));
            ImGui::End();
        }
        ImGui::End();
    }

    if (show_venus)
    {
        ImGui::Begin("Mars Settings", &show_venus, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &venus_distance_to_sun, 1.0f, 20.0f);
        ImGui::SliderFloat("Speed", &venus_rot_speed, 1.0f, 5.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(venus.rotation), -1.0f, 1.0f);;
        ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&venus_color));
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
    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Initialisation of window
    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Planets", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti-Aliasing

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
        double current_frame = glfwGetTime();
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
