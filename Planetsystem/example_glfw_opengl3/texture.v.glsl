#version 330 core

layout (location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

uniform vec4 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;
out vec2 vTexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vColor = color;
	vTexCoord = texCoord;
}
