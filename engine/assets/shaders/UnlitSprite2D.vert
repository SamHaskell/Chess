#version 410 core

layout (location = 0) in vec4 i_Position;
layout (location = 1) in vec4 i_Color;
layout (location = 2) in vec2 i_TexCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

void main() {
    gl_Position = i_Position;
    v_Color = i_Color;
}