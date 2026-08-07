//
// Created by kaloyan on 8/7/26.
//

#ifndef MATF_RG_PROJECT_RENDERTARGET_HPP
#define MATF_RG_PROJECT_RENDERTARGET_HPP

#include <string>
#include <vector>
#include "../glad/include/glad/glad.h"

namespace engine {
namespace graphics {
/**
 *  @class Bloom
 *  @brief Creates render target for engine drawing process. Supports drawing to multiple textures for postprocessing
 */
class RenderTarget {

public:

    RenderTarget() = default;
    /**
     * @brief Creates RenderTarget object with one output texture
     */
    RenderTarget(int width, int height);
    ~RenderTarget();

    /**
      * @brief Adds a single output texture to current RenderTarget
      */
    void add_color_texture();

    /**
      * @brief Resize all resources to given dimensions
      */
    void resize(int width, int height);

    /**
      * @brief Binds RenderTarget framebuffer.
      */
    void bind();

    /**
      * @brief Unbinds RenderTarget framebuffer.
      */
    void unbind();

    /**
      * @brief Returns framebuffer for this object
      */
    GLuint framebuffer() const {
        return m_fbo;
    }

    /**
      * @brief Returns texture at a given index for this object
      */
    GLuint texture(size_t index) const {
        return m_color_textures.at(index);
    }

    /**
      * @brief Returns all textures that belong to this object
      */
    const std::vector<GLuint> & textures() const {
        return m_color_textures;
    }
    /**
      * @brief Returns current width
      */
    int width() const {
        return m_width;
    }

    /**
      * @brief Returns current height
      */
    int height() const {
        return m_height;
    }
private:
    GLuint m_fbo = 0;
    GLuint m_depth_stencil = 0;
    std::vector<GLuint> m_color_textures;
    int m_width = 0;
    int m_height = 0;

    friend class GraphicsController;
};

}// namespace graphics
}// namespace engine

#endif//MATF_RG_PROJECT_RENDERTARGET_HPP
