#version 420

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;

layout(location = 0) out vec3 f_Normal;

layout(push_constant) uniform VertexPushConstant
{
	layout(offset = 0) mat4 mvp;
	layout(offset = 64) mat3 normal;
} vpc;

void main()
{
    gl_Position = vpc.mvp * vec4(v_Position, 1.0);
    f_Normal = normalize(vpc.normal * v_Normal);
}
