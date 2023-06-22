#include "Wigner/Graphics/Renderer.hpp"
#include "Wigner/Graphics/Shader.hpp"

#include <memory>

namespace Wigner {
    static glm::vec4 quad_vertices[4] = {
        {-0.5, -0.5, 0.0, 1.0},
        {-0.5,  0.5, 0.0, 1.0},
        { 0.5,  0.5, 0.0, 1.0},
        { 0.5, -0.5, 0.0, 1.0}
    };

    static glm::vec2 quad_texcoords[4] = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},
        {1.0, 0.0}
    };

    static u32 quad_indices[6] = {
        0, 1, 2, 2, 3, 0
    };

    static std::shared_ptr<Shader> shader_quad;

    static bool is_initialized;

    SceneData scene_begin(OrthographicCamera camera) {
        if (!is_initialized) {
            LOG_ERROR("Cannot begin scene without initialising the renderer!");
        }
        SceneData scene = {camera.ViewProjectionMatrix()};
        return scene;
    }

    void renderer_init() {
        is_initialized = true;
        shader_quad = Shader::Create("assets/shaders/UnlitSprite2D.vert", "assets/shaders/UnlitSprite2D.frag");
    }

    void renderer_shutdown() {
        is_initialized = false;
    }

    void draw_textured_quad(SceneData scene, Texture2D texture, Rect2D target_rect, Color color) {
        draw_textured_quad(scene, texture, target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, color);
    }

    void draw_textured_quad(SceneData scene, Texture2D texture, f32 x, f32 y, f32 width, f32 height, Color color) {

    }
    
    void draw_quad(SceneData scene, Rect2D target_rect, Color color) {
        draw_quad(scene, target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, color);
    }
    
    void draw_quad(SceneData scene, f32 x, f32 y, f32 width, f32 height, Color color) {
        Vertex2D vertices[4];
        glm::mat4 transform = glm::translate(glm::scale(glm::mat4(1.0), {width, height, 1.0f}), {x, y, 0.0f});
        for (int i = 0; i < 4; i++) {
            vertices[i].Position = transform * quad_vertices[i];
            vertices[i].Color = color;
            vertices[i].TexCoord = quad_texcoords[i];
        }
    }


}

