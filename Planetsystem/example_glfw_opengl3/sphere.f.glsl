#version 330 core
out vec4 FragColor;

uniform vec4 objectColor;

void main(void) {
  gl_FragColor = objectColor;
}
