#version 400

#define MAX_PARTICLES 200
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelMats[MAX_PARTICLES];
uniform mat4 ViewProjMat;

void main()
{
    Position = (ModelMats[gl_InstanceID] * VertexPos).xyz;
    Normal = (ModelMats[gl_InstanceID] * VertexNormal).xyz;
    gl_Position = ViewProjMat * ModelMats[gl_InstanceID] * VertexPos;
}