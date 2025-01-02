#version 410 core

layout (location = 0) in vec3 bezierCoordIn;
layout (location = 1) in vec3 normalEndIn;

out PosNorm {
    vec3 worldPos;
    vec3 normal;
} vs_out;

uniform mat4 cameraMtx;
uniform vec3 controlPoints[64];


vec3 GetControlPoint(int i, int j, int k)
{
    return controlPoints[i + 4*j + 16*k];
}


vec4 CubicBernsteinPolynomials(float t) {
    float oneMinusT = 1.0 - t;

    return vec4 (
    oneMinusT * oneMinusT * oneMinusT,
    3.f * oneMinusT * oneMinusT * t,
    3.f * oneMinusT * t * t,
    t * t * t
    );
}


vec3 WorldPosition(vec4 bernPol1, vec4 bernPol2, vec4 bernPol3)
{
    vec3 result = vec3(0.f, 0.f, 0.f);
    for (int i=0; i < 4; i++) {
        for (int j=0; j < 4; j++) {
            for (int k=0; k < 4; k++) {
                result += GetControlPoint(i, j, k) * bernPol1[i] * bernPol2[j] * bernPol3[k];
            }
        }
    }

    return result;
}


void main()
{
    vs_out.worldPos = WorldPosition(
        CubicBernsteinPolynomials(bezierCoordIn.x),
        CubicBernsteinPolynomials(bezierCoordIn.y),
        CubicBernsteinPolynomials(bezierCoordIn.z)
    );

    vec3 normalEnd = WorldPosition(
        CubicBernsteinPolynomials(normalEndIn.x),
        CubicBernsteinPolynomials(normalEndIn.y),
        CubicBernsteinPolynomials(normalEndIn.z)
    );

    vs_out.normal = normalize(normalEnd - vs_out.worldPos);

    gl_Position = cameraMtx * vec4(vs_out.worldPos, 1.f);
}
