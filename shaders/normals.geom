#version 410 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in PosNorm {
    vec3 worldPos;
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.1;

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * vec4(gs_in[index].worldPos, 1.f);
    EmitVertex();
    gl_Position = projection * (vec4(gs_in[index].worldPos, 1.f) + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}