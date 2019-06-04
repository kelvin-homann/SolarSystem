#version 330 core
uniform vec4 ourColor;

in vec3 Normal;  
in vec3 FragPos;  

void main(void) {
  gl_FragColor = ourColor;
}
