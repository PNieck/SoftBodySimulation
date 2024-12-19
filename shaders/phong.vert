#version 410 core

layout (location = 0) in vec3 posIn;
layout (location = 1) in vec3 normalIn;

out PosNorm {
    vec3 worldPos;
    vec3 normal;
} vs_out;

uniform mat4 modelMtx;
uniform mat4 modelInvMtx;
uniform mat4 viewMtx;
uniform mat4 projectionMtx;


void main()
{
    vs_out.worldPos = (modelMtx * vec4(posIn, 1.f)).xyz;

    vs_out.normal = normalize((transpose(modelInvMtx) * vec4(normalIn, 0.f)).xyz);

    gl_Position = projectionMtx * viewMtx * vec4(vs_out.worldPos, 1.f);
}
