#include "model.h"

void Model::SetModelViewProjection(int screen_height, int screen_width)
{
    //float angle = (ImGui::GetTime() / rot_speed) * 50;
    //glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    glm::mat4 _model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0f));
    glm::mat4 _view = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -cam_distance), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 _projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);

    glm::mat4 _mvp = _projection * _view * _model;

    //glm::mat4 translatedmvp = mvp * glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0));
}
