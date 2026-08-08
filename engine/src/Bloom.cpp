//
// Created by kaloyan on 8/7/26.
//

#include "engine/graphics/Bloom.hpp"

#include "glad/glad.h"

namespace engine {
namespace graphics {

Bloom::~Bloom() {
}
void Bloom::destroy() {
    GLint current_fbo_id = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);

    glDeleteFramebuffers(1, &(m_fbo[0]));
    glDeleteFramebuffers(1, &(m_fbo[1]));

    glDeleteTextures(static_cast<GLsizei>(2), m_texture);
    if (current_fbo_id == m_fbo[0] || current_fbo_id == m_fbo[1])
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
Bloom::Bloom(int width, int height) {

    GLint current_fbo_id = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);
    for (int i = 0; i < 2; i++) {
        m_height = height;
        m_width = width;
        glGenFramebuffers(1, &m_fbo[i]);

        glGenTextures(1, &m_texture[i]);
        glBindTexture(GL_TEXTURE_2D, m_texture[i]);
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture[i], 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, current_fbo_id);
}

void Bloom::resize(uint32_t width, uint32_t height) {
    for (int i = 0; i < 2; i++) {
        if (height != m_height || width != m_width) {
            glBindTexture(GL_TEXTURE_2D, m_texture[i]);
            glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        }
    }
}

} // graphics
} // engine