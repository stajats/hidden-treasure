//
// Created by kaloyan on 8/7/26.
//

#ifndef MATF_RG_PROJECT_BLOOM_HPP
#define MATF_RG_PROJECT_BLOOM_HPP
#include <cstdint>

namespace engine {
namespace graphics {

class Bloom {
public:

    uint32_t fbo(int i) const {
        return m_fbo[i];
    }
    uint32_t texture(int i) const {
        return m_texture[i];
    }
    uint32_t width() const {
        return m_width;
    }
    uint32_t height() const {
        return m_height;
    }

    ~Bloom();
    Bloom(int width, int height);
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
