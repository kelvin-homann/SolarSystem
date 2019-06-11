#include "main.h"

// Legacy glf3.lib support
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void InitResources()
{
    // Light
    light.position = sun.position;
    light.color = glm::vec3(1.0f, 1.0f, 0.8f);
    light.diffuse = glm::vec3(1.0f, 1.0f, 0.8f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Camera
    camera.position = initPos;
    camera.SetPitch(-42.f);
    camera.Yaw = 0.f;

    // Sun
    sun.SetShader("sphere.v.glsl", "sphere.f.glsl");
    sun.BindBuffers();
    sun.loadTexture("textures/sun.jpg");
    sun.material.ambient = glm::vec3(1.0f, 1.0f, 0.8f);
    sun.material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    sun.material.shininess = 32.0f;

    // Earth
    earth.SetShader("sphere.v.glsl", "sphere.f.glsl");
    earth.BindBuffers();
    earth.loadTexture("textures/earth.jpg");
    earth.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    earth.material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    earth.material.specular = glm::vec3(0.0f, 0.5f, 1.0f); 
    earth.material.shininess = 32.0f;

    earth.m_distanceToParent = 70.0f;
    earth.m_rotSpeed = 1.0f;

    // Earth Moon
    earth_moon.SetShader("sphere.v.glsl", "sphere.f.glsl");
    earth_moon.BindBuffers();
    earth_moon.loadTexture("textures/moon.jpg");
    earth_moon.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    earth_moon.material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    earth_moon.material.specular = glm::vec3(0.0f, 0.5f, 1.0f);
    earth_moon.material.shininess = 32.0f;
    earth_moon.m_distanceToParent = 15.0f;
    earth_moon.m_rotSpeed = 6.0f;

    // Mars
    mars.SetShader("sphere.v.glsl", "sphere.f.glsl");
    mars.BindBuffers();
    mars.loadTexture("textures/mars.jpg");
    mars.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    mars.material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    mars.material.specular = glm::vec3(0.0f, 0.5f, 1.0f);
    mars.material.shininess = 32.0f;
    mars.m_distanceToParent = 35.0f;
    mars.m_rotSpeed = 0.7f;

    // Mars Moon
    mars_moon.SetShader("sphere.v.glsl", "sphere.f.glsl");
    mars_moon.BindBuffers();
    mars_moon.loadTexture("textures/moon.jpg");
    mars_moon.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    mars_moon.material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    mars_moon.material.specular = glm::vec3(0.0f, 0.5f, 1.0f);
    mars_moon.material.shininess = 32.0f;
    mars_moon.m_distanceToParent = 10.0f;
    mars_moon.m_rotSpeed = 2.0f;

    // Venus
    venus.SetShader("sphere.v.glsl", "sphere.f.glsl");
    venus.BindBuffers();
    venus.loadTexture("textures/venus.jpg");
    venus.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    venus.material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    venus.material.specular = glm::vec3(0.0f, 0.5f, 1.0f);
    venus.material.shininess = 32.0f;

    venus.m_distanceToParent = 55.0f;
    venus.m_rotSpeed = 0.5f;
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

    glm::vec3 earthPosRel = glm::vec3(sin(elapsedTimeScaled * earth.m_rotSpeed), 0.f, cos(elapsedTimeScaled * earth.m_rotSpeed)) * earth.m_distanceToParent;
    glm::vec3 earthPosAbs = sun.position + earthPosRel;

    glm::vec3 moonPosRel = glm::vec3(sin(elapsedTimeScaled * earth_moon.m_rotSpeed), 0.f, cos(elapsedTimeScaled * earth_moon.m_rotSpeed)) * earth_moon.m_distanceToParent;
    glm::vec3 moonPosAbs = earthPosAbs + moonPosRel;

    glm::vec3 marsPosRel = glm::vec3(sin(elapsedTimeScaled * mars.m_rotSpeed), 0.f, cos(elapsedTimeScaled * mars.m_rotSpeed)) * mars.m_distanceToParent;
    glm::vec3 marsPosAbs = sun.position + marsPosRel;

    glm::vec3 marsmoonPosRel = glm::vec3(sin(elapsedTimeScaled * mars_moon.m_rotSpeed), 0.f, cos(elapsedTimeScaled * mars_moon.m_rotSpeed)) * mars_moon.m_distanceToParent;
    glm::vec3 marsmoonPosAbs = marsPosAbs + marsmoonPosRel;

    glm::vec3 venusPosRel = glm::vec3(sin(elapsedTimeScaled * venus.m_rotSpeed), 0.f, cos(elapsedTimeScaled * venus.m_rotSpeed)) * venus.m_distanceToParent;
    glm::vec3 venusPosAbs = sun.position + venusPosRel;

    glm::mat4 view = camera.GetViewMatrix(); // Camera
    projection = glm::perspective(glm::radians(camera.Zoom), (float)screen_width / (float)screen_height, near_plane, far_plane); // Projection

    // Sun
    sun.BindShader();
    model = glm::translate(glm::mat4(1.0f), sun.position); // Settings Sun Position

    glBindTexture(GL_TEXTURE_2D, sun.m_textureID);
    sun.GetShader().setInt("ownTexture", sun.m_textureID);
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
    sun.Render();
    
    // Earth
    angle = elapsedTimeScaled / earth.m_rotSpeed;

    earth.BindShader();
    model = glm::translate(glm::mat4(1.0f), earthPosAbs);
    model = glm::rotate(model, angle, earth.rotation);

    glBindTexture(GL_TEXTURE_2D, earth.m_textureID);
    earth.GetShader().setInt("ownTexture", earth.m_textureID);
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
    earth.Render();

    // Earth Moon
    earth_moon.BindShader();
    model = glm::translate(glm::mat4(1.0f), moonPosAbs);
    model = glm::rotate(model, angle, earth_moon.rotation);

    glBindTexture(GL_TEXTURE_2D, earth_moon.m_textureID);
    earth_moon.GetShader().setInt("ownTexture", earth_moon.m_textureID);
    earth_moon.GetShader().setVec3("light.ambient", light.color);
    earth_moon.GetShader().setVec3("light.diffuse", light.diffuse);
    earth_moon.GetShader().setVec3("light.specular", light.specular);

    // material properties
    earth_moon.GetShader().setVec3("material.ambient", earth_moon.material.ambient);
    earth_moon.GetShader().setVec3("material.diffuse", earth_moon.material.diffuse);
    earth_moon.GetShader().setVec3("material.specular", earth_moon.material.specular);
    earth_moon.GetShader().setFloat("material.shininess", earth_moon.material.shininess);

    earth_moon.GetShader().setVec3("light.position", light.position);
    earth_moon.GetShader().setVec3("viewPos", camera.position);
    earth_moon.GetShader().setMat4("projection", projection);
    earth_moon.GetShader().setMat4("view", view);
    earth_moon.GetShader().setMat4("model", model);
    earth_moon.Render();

    // Mars
    angle = elapsedTimeScaled / mars.m_rotSpeed;

    mars.BindShader();
    model = glm::translate(glm::mat4(1.0f), marsPosAbs);
    model = glm::rotate(model, angle, mars.rotation);

    glBindTexture(GL_TEXTURE_2D, mars.m_textureID);
    mars.GetShader().setInt("ownTexture", mars.m_textureID);
    mars.GetShader().setVec3("light.ambient", light.color);
    mars.GetShader().setVec3("light.diffuse", light.diffuse);
    mars.GetShader().setVec3("light.specular", light.specular);

    // material properties
    mars.GetShader().setVec3("material.ambient", mars.material.ambient);
    mars.GetShader().setVec3("material.diffuse", mars.material.diffuse);
    mars.GetShader().setVec3("material.specular", mars.material.specular);
    mars.GetShader().setFloat("material.shininess", mars.material.shininess);

    mars.GetShader().setVec3("light.position", light.position);
    mars.GetShader().setVec3("viewPos", camera.position);
    mars.GetShader().setMat4("projection", projection);
    mars.GetShader().setMat4("view", view);
    mars.GetShader().setMat4("model", model);

    mars.SetColor(mars_color);
    mars.Render();

    // Mars Moon
    mars_moon.BindShader();
    model = glm::translate(glm::mat4(1.0f), marsmoonPosAbs);
    model = glm::rotate(model, angle, mars_moon.rotation);

    glBindTexture(GL_TEXTURE_2D, mars_moon.m_textureID);
    mars_moon.GetShader().setInt("ownTexture", mars_moon.m_textureID);
    mars_moon.GetShader().setVec3("light.ambient", light.color);
    mars_moon.GetShader().setVec3("light.diffuse", light.diffuse);
    mars_moon.GetShader().setVec3("light.specular", light.specular);

    // material properties
    mars_moon.GetShader().setVec3("material.ambient", mars_moon.material.ambient);
    mars_moon.GetShader().setVec3("material.diffuse", mars_moon.material.diffuse);
    mars_moon.GetShader().setVec3("material.specular", mars_moon.material.specular);
    mars_moon.GetShader().setFloat("material.shininess", mars_moon.material.shininess);

    mars_moon.GetShader().setVec3("light.position", light.position);
    mars_moon.GetShader().setVec3("viewPos", camera.position);
    mars_moon.GetShader().setMat4("projection", projection);
    mars_moon.GetShader().setMat4("view", view);
    mars_moon.GetShader().setMat4("model", model);

    mars_moon.SetColor(moon_color);
    mars_moon.Render();

    // Venus
    angle = elapsedTimeScaled / mars.m_rotSpeed;

    venus.BindShader();
    model = glm::translate(glm::mat4(1.0f), venusPosAbs);
    model = glm::rotate(model, angle, venus.rotation);

    glBindTexture(GL_TEXTURE_2D, venus.m_textureID);
    venus.GetShader().setInt("ownTexture", venus.m_textureID);
    venus.GetShader().setVec3("light.ambient", light.color);
    venus.GetShader().setVec3("light.diffuse", light.diffuse);
    venus.GetShader().setVec3("light.specular", light.specular);

    // material properties
    venus.GetShader().setVec3("material.ambient", venus.material.ambient);
    venus.GetShader().setVec3("material.diffuse", venus.material.diffuse);
    venus.GetShader().setVec3("material.specular", venus.material.specular);
    venus.GetShader().setFloat("material.shininess", venus.material.shininess);

    venus.GetShader().setVec3("light.position", light.position);
    venus.GetShader().setVec3("viewPos", camera.position);
    venus.GetShader().setMat4("projection", projection);
    venus.GetShader().setMat4("view", view);
    venus.GetShader().setMat4("model", model);
    venus.Render();
}

void RenderMenu()
{
    const char* shading_elements[]{ "Default", "Wireframe" };
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Shading");
    ImGui::Combo("", &rendermode_selected_item, shading_elements, IM_ARRAYSIZE(shading_elements));

    ImGui::Separator();
    ImGui::Text("Time");
    ImGui::SliderFloat("Time", &timeScale, 0.f, 10.f, "%.2f");

    ImGui::Text("Camera");
    ImGui::InputFloat3("Camera Position", glm::value_ptr(camera.position), 2);
    ImGui::SliderFloat("Camera Pitch", &camera.Pitch, -89.f, 89.f, "%.2f");
    ImGui::SliderFloat("Camera Yaw", &camera.Yaw, -89.f, 89.f, "%.2f");

    ImGui::Separator();

    ImGui::Text("Objects");
    ImGui::Checkbox("Sun", &show_sun); ImGui::SameLine(100);
    ImGui::Checkbox("Earth", &show_earth); 
    ImGui::Checkbox("Mars", &show_mars); ImGui::SameLine(100);
    ImGui::Checkbox("Venus", &show_venus); 

    if (show_sun)
    {
        ImGui::Begin("Sun Settings", &show_sun, ImGuiWindowFlags_NoCollapse);
        ImGui::InputFloat3("Position", glm::value_ptr(sun.position), 2);

        ImGui::Separator();

        // Material
        ImGui::Text("Material");
        ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&sun.material.ambient));
        ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&sun.material.diffuse));
        ImGui::SliderFloat3("Specular", glm::value_ptr(sun.material.specular), 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &sun.material.shininess, 1.0f, 50.0f);

        ImGui::Separator();

        ImGui::Text("Light");
        ImGui::SliderFloat3("Light Position", glm::value_ptr(light.position), -50.0f, 50.0f);
        ImGui::ColorEdit3("Light Color", reinterpret_cast<float*>(&light.color));
        ImGui::ColorEdit3("Light Diffuse", reinterpret_cast<float*>(&light.diffuse));
        ImGui::SliderFloat("Light Specular", glm::value_ptr(light.specular), 0.0f, 1.0f);
        ImGui::End();
    }

    if (show_earth)
    {
        ImGui::Begin("Earth Settings", &show_earth, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &earth.m_distanceToParent, 1.0f, 100.0f);
        ImGui::SliderFloat("Speed", &earth.m_rotSpeed, 1.0f, 5.0f);
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
            ImGui::SliderFloat("Planet Distance", &earth_moon.m_distanceToParent, 1.0f, 100.0f);
            ImGui::SliderFloat("Rotation Speed", &earth_moon.m_rotSpeed, 1.0f, 20.0f);
            // Material
            ImGui::Text("Material");
            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&earth_moon.material.ambient));
            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&earth_moon.material.diffuse));
            ImGui::SliderFloat3("Specular", glm::value_ptr(earth_moon.material.specular), 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &earth_moon.material.shininess, 1.0f, 50.0f);

            ImGui::End();
        }
        ImGui::End();
    }

    if (show_mars)
    {
        ImGui::Begin("Mars Settings", &show_mars, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &mars.m_distanceToParent, 1.0f, 100.0f);
        ImGui::SliderFloat("Speed", &mars.m_rotSpeed, 1.0f, 5.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(mars.rotation), -1.0f, 1.0f);;
        ImGui::Separator();

        // Material
        ImGui::Text("Material");
        ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&mars.material.ambient));
        ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&mars.material.diffuse));
        ImGui::SliderFloat3("Specular", glm::value_ptr(mars.material.specular), 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &mars.material.shininess, 1.0f, 50.0f);

        ImGui::Separator();
        ImGui::Checkbox("Moon", &show_mars_moon);

        if (show_mars_moon)
        {
            ImGui::Begin("Mars Moon Settings", &show_mars_moon, ImGuiWindowFlags_NoCollapse);
            ImGui::SliderFloat("Planet Distance", &mars_moon.m_distanceToParent, 1.0f, 100.0f);
            ImGui::SliderFloat("Rotation Speed", &mars_moon.m_rotSpeed, 1.0f, 20.0f);
            // Material
            ImGui::Text("Material");
            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&mars_moon.material.ambient));
            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&mars_moon.material.diffuse));
            ImGui::SliderFloat3("Specular", glm::value_ptr(mars_moon.material.specular), 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &mars_moon.material.shininess, 1.0f, 50.0f);

            ImGui::End();
        }
        ImGui::End();
    }

    if (show_venus)
    {
        ImGui::Begin("Venus Settings", &show_venus, ImGuiWindowFlags_NoCollapse);
        ImGui::SliderFloat("Sun Distance", &venus.m_distanceToParent, 1.0f, 100.0f);
        ImGui::SliderFloat("Speed", &venus.m_rotSpeed, 1.0f, 5.0f);
        ImGui::SliderFloat3("Rotation", glm::value_ptr(venus.rotation), -1.0f, 1.0f);;
        ImGui::Separator();

        // Material
        ImGui::Text("Material");
        ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&venus.material.ambient));
        ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&venus.material.diffuse));
        ImGui::SliderFloat3("Specular", glm::value_ptr(venus.material.specular), 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &venus.material.shininess, 1.0f, 50.0f);

        ImGui::Separator();

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

// Cleanup
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
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 410";
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
    icons[0].pixels = SOIL_load_image("textures/icon.png", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
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

        glfwMakeContextCurrent(window);
        processInput(window);

        glfwPollEvents();
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        if (screen_width == 0 || screen_height == 0)
            continue;

        Render();
        RenderMenu();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Switching between render modes
        switch (rendermode_selected_item) {
        case 0:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE); // Anti-Aliasing

        glfwSwapBuffers(window);        
    }

    // Cleanup    
    FreeResources(window);

    return 0;
}
