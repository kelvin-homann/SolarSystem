#version 330 core
attribute vec3 coord3d;
attribute vec3 v_color;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 anim;

void main()
{
    FragPos = vec3(model * vec4(coord3d, 1.0));
    Normal = mat3(transpose(inverse(model))) * v_color;  
    gl_Position = projection * view * model * vec4(coord3d, 1.0);
}
