#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Graphics/RenderTypes.hpp"
#include "Wigner/Graphics/Texture2D.hpp"

namespace Wigner {
    void renderer_init();
    void renderer_shutdown();
    
    void draw_quad(f32 x, f32 y, f32 width, f32 height, Color color);
    void draw_quad(Rect2D target_rect, Color color);
    void draw_textured_quad(Texture2D texture, f32 x, f32 y, f32 width, f32 height, Color color);
    void draw_textured_quad(Texture2D texture, Rect2D target_rect, Color color);
}