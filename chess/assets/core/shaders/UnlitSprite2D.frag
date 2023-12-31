#version 410 core

in vec4 v_Color;
in vec2 v_TexCoord;

out vec4 f_Color;

uniform sampler2D u_Texture;

void main() {
    f_Color = texture(u_Texture, v_TexCoord) * v_Color;
}