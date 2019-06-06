#version 330 core
in vec4 vVertex; // Vertex-Position in Objektkoordinaten
in vec3 vNormal; // Normalen-Vektor in Objektkoordinaten

// Uniform-Eingabe-Werte
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

// Ausgabe-Werte
smooth out vec3 Position;
smooth out vec3 Normal;

void main(void)
{
    
}


