attribute vec3 coord3d;
attribute vec3 v_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 anim;

void main()
{
    gl_Position = projection * view * model * vec4(coord3d, 1.0);
}
