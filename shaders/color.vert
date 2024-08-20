#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 v_color;


layout(std140, binding = 0) uniform modelMatrix {
    mat4 model;
    mat4 view;
    mat4 projection;
};


void main()
{
    v_color = color;
    gl_Position = projection * view * model * position;
}
