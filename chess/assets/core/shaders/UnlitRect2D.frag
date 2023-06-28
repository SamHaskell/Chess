#version 410 core

in vec4 v_Color;
in vec2 v_TexCoord;

out vec4 f_Color;

void main() {
    f_Color = v_Color;
}