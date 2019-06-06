#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>

#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL // Debug
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp" // Debug

#include "soil.h"
#include "shader.h"
#include "sphere.h"
#include "camera.h"
#include "light.h"

int screen_width = 1280;
int screen_height = 720;

ImVec4 background_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

// Sun
Sphere sun(10.0f);

// Earth
Sphere earth(3.f);
Sphere earth_moon(1.0f);
glm::vec4 moon_color(0.5f, 0.5f, 0.5f, 1.0f);

// Mars
glm::vec4 mars_color(1.0f, 0.5f, 0.0f, 1.0f);
Sphere mars(2.5f);
Sphere mars_moon(1.0f);

// Venus
glm::vec4 venus_color(1.0f, 0.3f, 0.15f, 1.0f);
Sphere venus(3.0f);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 initPos(-25, 22, 0);
glm::vec3 cameraOffset(0, 40, 0);
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
int rendermode_selected_item;

glm::mat4 anim = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 mvp = glm::mat4(1.0f);

// Light
Light light = Light();
