#include "model.h"
#include "imgui.h"

glm::mat4 Model::GetMVP(int screen_height, int screen_width, float rot_speed)
{
    float angle = (ImGui::GetTime() / rot_speed) * 50;
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -cam_distance), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model;

    glm::mat4 translatedmvp = mvp * glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0));

    return translatedmvp;
}

void Model::SetShader(const char* vertexPath, const char* fragmentPath)
{
    shader = Shader(vertexPath, fragmentPath);
}
