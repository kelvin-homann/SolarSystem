attribute vec3 coord3d;
attribute vec3 v_color;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(coord3d, 1.0);
}
