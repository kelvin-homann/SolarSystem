#version 330 core

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D ownTexture;

out vec4 color;
out vec4 fragColor;

void main()
{
   fragColor = texture(ownTexture, vTexCoord);
}
