#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Graphics/RenderTypes.hpp"
#include "Wigner/Graphics/Texture2D.hpp"
#include "Wigner/Graphics/Cameras.hpp"

namespace Wigner
{
    void renderer_init();
    void renderer_shutdown();
    SceneData scene_begin(OrthographicCamera camera);
    void draw_quad(SceneData scene, f32 x, f32 y, f32 width, f32 height, Color color);
    void draw_quad(SceneData scene, Rect2D target_rect, Color color);
    void draw_textured_quad(SceneData scene, const Texture2D &texture, f32 x, f32 y, f32 width, f32 height, Color color);
    void draw_textured_quad(SceneData scene, const Texture2D &texture, Rect2D target_rect, Color color);
}