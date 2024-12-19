#version 410 core


in PosNorm {
    vec3 worldPos;
    vec3 normal;
} vs_out;


uniform vec4 fragmentColor;
uniform vec3 lightPos;
uniform vec3 camPos;


out vec4 FragColor;


const vec3 ambientColor = vec3(0.2f, 0.2f, 0.2f);
const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
const float kd = 0.5, ks = 0.2f, m = 100.0f;


void main()
{
    vec3 viewVecN = normalize(camPos - vs_out.worldPos);
    vec3 normalN = normalize(vs_out.normal);

    // Ambient reflection
    vec3 color = fragmentColor.xyz * ambientColor;

    vec3 lightVec = normalize(lightPos - vs_out.worldPos);
    vec3 halfVec = normalize(viewVecN + lightVec);

    // Diffuse reflection
    color += lightColor * fragmentColor.xyz * kd * clamp(dot(normalN, lightVec), 0.f, 1.f);

    // Specular reflection
    color += lightColor * ks * pow(clamp(dot(normalN, halfVec), 0.f, 1.f), m);

    FragColor = vec4(clamp(color, 0.f, 1.f), fragmentColor.w);
}
