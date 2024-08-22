#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

layout(std140, binding = 0) uniform modelBlock {
    mat4 model;
};


layout(std140, binding = 1) uniform viewProjectionBlock {
    mat4 view;
    mat4 projection;
};

layout(location = 0) out vec3 v_color;

void main()
{
    v_color = color;
    gl_Position = projection * view * model * position;
}
