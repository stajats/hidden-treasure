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

class RenderTarget {

public:

    RenderTarget() = default;
    RenderTarget(int width, int height);
    ~RenderTarget();

    /**
      * @brief
      */
    void addColorTexture();

    /**
      * @brief
      */
    void create(int width, int height);

    /**
      * @brief
      */
    void resize(int width, int height);

    /**
      * @brief
      */
    void bind();

    /**
      * @brief
      */
    void unbind();

    /**
      * @brief
      */
    GLuint framebuffer() const {
        return m_fbo;
    }
    GLuint texture(size_t index) const {
        return m_color_textures.at(index);
    }
    const std::vector<GLuint> & textures() const {
        return m_color_textures;
    }
    int width() const {
        return m_width;
    }

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
