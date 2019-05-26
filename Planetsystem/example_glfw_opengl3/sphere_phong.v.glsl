in vec3 vVertex;
in vec3 vNormal;

uniform mat4 mv;
uniform mat4 mvp;
uniform mat3 normalm;

smooth out vec3 position;
smooth out vec3 normal;

void main(void)
{
    gl_Position = mvp * vVertex;
    vec4 pos = mv * vVertex;
    position = pos.xyz / pos.w;
    normal = normalize(normalm * vNormal);
}
