#version 410  // GLSL-Version 4.1
//Eingabe-Werte pro Vertex
in vec4 vColor;     
// vom Rasterizer interpolierte Fragmentfarbe
//Ausgabe-Werte pro Fragment
out vec4 FragColor; 
// Fragment-Farbe
void main(void)
{
    FragColor = vColor;
}