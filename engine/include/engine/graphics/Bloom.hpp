//
// Created by kaloyan on 8/7/26.
//

#ifndef MATF_RG_PROJECT_BLOOM_HPP
#define MATF_RG_PROJECT_BLOOM_HPP
#include <cstdint>

namespace engine {
namespace graphics {
/**
 *  @class Bloom
 *  @brief Creates and stores framebuffers and textures needed to achieve bloom effect.
 */
class Bloom {
public:
    /**
      * @brief Returns framebuffer at a given index.
      */
    uint32_t fbo(int i) const {
        return m_fbo[i];
    }

    /**
      * @brief Returns texture at a given index.
      */
    uint32_t texture(int i) const {
        return m_texture[i];
    }
    /**
      * @brief Returns current width of bloom textures.
      */
    uint32_t width() const {
        return m_width;
    }

    /**
      * @brief Returns current height of bloom texture
      */
    uint32_t height() const {
        return m_height;
    }

    ~Bloom();
    Bloom(int width, int height);

    /**
      * @brief Resizes textures for given dimensions.
      */
    void resize(uint32_t width, uint32_t height);

private:
    uint32_t m_fbo[2];
    uint32_t m_texture[2];
    uint32_t m_width;
    uint32_t m_height;
};

}// namespace graphics
}// namespace engine

#endif//MATF_RG_PROJECT_BLOOM_HPP
