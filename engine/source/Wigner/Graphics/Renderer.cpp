#include "Wigner/Graphics/Renderer.hpp"
#include "Wigner/Graphics/Shader.hpp"

#include <memory>

namespace Wigner
{
    static glm::vec4 quad_vertices[4] = {
        {0.0, 0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0, 1.0},
        {1.0, 1.0, 0.0, 1.0},
        {1.0, 0.0, 0.0, 1.0}};

    static glm::vec2 quad_texcoords[4] = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},
        {1.0, 0.0}};

    static u32 quad_indices[6] = {
        0, 1, 2, 2, 3, 0};

    static std::shared_ptr<Shader> shader_rect;
    static std::shared_ptr<Shader> shader_sprite;

    static bool is_initialized;

    SceneData scene_begin(OrthographicCamera camera)
    {
        if (!is_initialized)
        {
            LOG_ERROR("Cannot begin scene without initialising the renderer!");
        }
        SceneData scene = {camera.ViewMatrix, camera.ProjectionMatrix};
        return scene;
    }

    void renderer_init()
    {
        is_initialized = true;
        shader_rect = Shader::Create("assets/core/shaders/UnlitRect2D.vert", "assets/core/shaders/UnlitRect2D.frag");
        shader_sprite = Shader::Create("assets/core/shaders/UnlitSprite2D.vert", "assets/core/shaders/UnlitSprite2D.frag");
        glDisable(GL_CULL_FACE);
    }

    void renderer_shutdown()
    {
        is_initialized = false;
    }

    void draw_textured_quad(SceneData scene, const std::shared_ptr<Texture2D> texture, Rect2D target_rect, Color color)
    {
        draw_textured_quad(scene, texture, target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, color);
    }

    void draw_textured_quad(SceneData scene, const std::shared_ptr<Texture2D> texture, f32 x, f32 y, f32 width, f32 height, Color color)
    {
        Vertex2D vertices[4];
        glm::mat4 translation = glm::translate(glm::mat4(1.0), {x, y, 0.0f});
        glm::mat4 scale = glm::scale(glm::mat4(1.0), {width, height, 1.0f});

        for (int i = 0; i < 4; i++)
        {
            vertices[i].Position = translation * scale * quad_vertices[i];
            vertices[i].Color = color;
            vertices[i].TexCoord = quad_texcoords[i];
        }  

        u32 vao, vbo, ibo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, TexCoord));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        texture->Bind();

        shader_sprite->Bind();
        shader_sprite->SetUniformMat4("u_View", glm::mat4(1.0f));
        shader_sprite->SetUniformMat4("u_Projection", glm::mat4(1.0f));
        shader_sprite->SetUniformMat4("u_View", scene.ViewMatrix);
        shader_sprite->SetUniformMat4("u_Projection", scene.ProjectionMatrix);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void draw_quad(SceneData scene, Rect2D target_rect, Color color)
    {
        draw_quad(scene, target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, color);
    }

    void draw_quad(SceneData scene, f32 x, f32 y, f32 width, f32 height, Color color)
    {
        Vertex2D vertices[4];
        glm::mat4 translation = glm::translate(glm::mat4(1.0), {x, y, 0.0f});
        glm::mat4 scale = glm::scale(glm::mat4(1.0), {width, height, 1.0f});

        for (int i = 0; i < 4; i++)
        {
            vertices[i].Position = translation * scale * quad_vertices[i];
            vertices[i].Color = color;
            vertices[i].TexCoord = quad_texcoords[i];
        }

        u32 vao, vbo, ibo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, TexCoord));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader_rect->Bind();

        shader_rect->SetUniformMat4("u_View", glm::mat4(1.0f));
        shader_rect->SetUniformMat4("u_Projection", glm::mat4(1.0f));

        shader_rect->SetUniformMat4("u_View", scene.ViewMatrix);
        shader_rect->SetUniformMat4("u_Projection", scene.ProjectionMatrix);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }
}
