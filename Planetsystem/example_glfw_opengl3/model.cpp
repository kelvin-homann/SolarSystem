#include "model.h"
#include "imgui.h"

void Model::SetShader(const char* vertexPath, const char* fragmentPath)
{
    shader = Shader(vertexPath, fragmentPath);
}
