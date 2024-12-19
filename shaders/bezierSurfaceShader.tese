#version 410 core

layout (quads, equal_spacing) in;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;


out PosNorm {
    vec3 worldPos;
    vec3 normal;
} ts_out;


vec4 CubicBernsteinPolynomials(float t) {
    float oneMinusT = 1.0 - t;

    return vec4 (
        oneMinusT * oneMinusT * oneMinusT,
        3.f * oneMinusT * oneMinusT * t,
        3.f * oneMinusT * t * t,
        t * t * t
    );
}


vec3 Derivative(float t, vec3 a0, vec3 a1, vec3 a2, vec3 a3) {
    float oneMinusT = 1.0 - t;

    return -3.f * oneMinusT * oneMinusT * a0 +
           (3.f * oneMinusT * oneMinusT - 6.f * t * oneMinusT) * a1 +
           (6.f * t * oneMinusT - 3.f * t * t) * a2 +
           3.f * t * t * a3;
}


void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    v *= float(gl_TessLevelOuter[0]) / float(gl_TessLevelOuter[0] - 1);

    // Bernstein polynomials
    vec4 bu = CubicBernsteinPolynomials(u);
    vec4 bv = CubicBernsteinPolynomials(v);

    // Control points
    vec3 p00 = gl_in[0].gl_Position.xyz;
    vec3 p01 = gl_in[1].gl_Position.xyz;
    vec3 p02 = gl_in[2].gl_Position.xyz;
    vec3 p03 = gl_in[3].gl_Position.xyz;

    vec3 p10 = gl_in[4].gl_Position.xyz;
    vec3 p11 = gl_in[5].gl_Position.xyz;
    vec3 p12 = gl_in[6].gl_Position.xyz;
    vec3 p13 = gl_in[7].gl_Position.xyz;

    vec3 p20 = gl_in[8].gl_Position.xyz;
    vec3 p21 = gl_in[9].gl_Position.xyz;
    vec3 p22 = gl_in[10].gl_Position.xyz;
    vec3 p23 = gl_in[11].gl_Position.xyz;

    vec3 p30 = gl_in[12].gl_Position.xyz;
    vec3 p31 = gl_in[13].gl_Position.xyz;
    vec3 p32 = gl_in[14].gl_Position.xyz;
    vec3 p33 = gl_in[15].gl_Position.xyz;

    // Result of bu*p
    vec3 a0 = bu.x*p00 + bu.y*p10 + bu.z*p20 + bu.w*p30;
    vec3 a1 = bu.x*p01 + bu.y*p11 + bu.z*p21 + bu.w*p31;
    vec3 a2 = bu.x*p02 + bu.y*p12 + bu.z*p22 + bu.w*p32;
    vec3 a3 = bu.x*p03 + bu.y*p13 + bu.z*p23 + bu.w*p33;

    // pos = bu * p * bv^T
    vec3 pos = a0*bv.x + a1*bv.y + a2*bv.z + a3*bv.w;

    // Tangent to surface
    vec3 tan = Derivative(
        u,
        bv.x*p00 + bv.y*p01 + bv.z*p02 + bv.w*p03,
        bv.x*p10 + bv.y*p11 + bv.z*p12 + bv.w*p13,
        bv.x*p20 + bv.y*p21 + bv.z*p22 + bv.w*p23,
        bv.x*p30 + bv.y*p31 + bv.z*p32 + bv.w*p33
    );

    // Bitangent to surface
    vec3 bitan = Derivative(v, a0, a1, a2, a3);

    ts_out.normal =  normalize(mat3(transpose(inverse(view * model))) * cross(bitan, tan));
    ts_out.worldPos = (view * model * vec4(pos, 1.0f)).xyz;

    gl_Position = projection * vec4(ts_out.worldPos, 1.f);
}
