#version 410
in vec3 vVertex;
in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalM;

uniform MaterialParams{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

uniform LightParams{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 halfVector;
} light;

out vec4 vColor;

void main()
{
    mat4 MVP = projection * view * model;
    vec4 emissive = material.emission;
    vec4 ambient = material.ambient * light.ambient;

    vec3 L = normalize(vec3(light.position));

    vec3 N = normalM * vNormal;
    float diffuseLight = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    if (diffuseLight > 0){
        vec4diff = material.diffuse * light.diffuse;
        diffuse = diffuseLight * diff;
        vec3 H = normalize(light.halfVector);
        floatspecLight = pow(max(dot(H, N), 0), material.shininess);
        vec4spec = material.specular * light.specular;
        specular = specLight * spec;
    }
    vColor = emissiv + ambient + diffuse + specular; 
    //Vertex aus Objekt- in Projektionskoordinaten
    glPosition = MVP * vVertex;
}

