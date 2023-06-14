#include "Wigner/Graphics/Renderer.hpp"

namespace Wigner {
    static glm::vec4 quad_vertices[4] = {
        {-0.5, -0.5, 0.0, 1.0},
        {-0.5,  0.5, 0.0, 1.0},
        { 0.5,  0.5, 0.0, 1.0},
        { 0.5, -0.5, 0.0, 1.0}
    };

    static u32 quad_indices[6] = {
        0, 1, 2, 2, 3, 0
    };

    static bool s_initialised = false;

    void renderer_init() {
        s_initialised = true;
    }

    void renderer_shutdown() {
        s_initialised = false;
    }

    void draw_textured_quad(Texture2D texture, Rect2D target_rect, Color color) {
        draw_textured_quad(texture, target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, color);
    }

    void draw_textured_quad(Texture2D texture, f32 x, f32 y, f32 width, f32 height, Color color) {

    }
    
    void draw_quad(Rect2D target_rect, Color color) {
        draw_quad(target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, color);
    }
    
    void draw_quad(f32 x, f32 y, f32 width, f32 height, Color color) {

    }


}

