//
// Created by kaloyan on 8/7/26.
//

#include "glad/glad.h"
#include "engine/graphics/RenderTarget.hpp"
#include "engine/util/Errors.hpp"

namespace engine {
namespace graphics {

void RenderTarget::create_texture(unsigned& texture, int width, int height) {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

RenderTarget::RenderTarget(int width, int height) {

    GLint current_fbo_id = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);

    m_width = width;
    m_height = height;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    create_texture(m_texture_primary, width, height);
    create_texture(m_texture_secondary, width, height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, m_texture_primary, 0);

    std::array<GLenum, 1> attachments = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

    glGenRenderbuffers(1, &m_depth_stencil);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_stencil);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw util::Error("RenderTarget incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, current_fbo_id);
}

void RenderTarget::destroy() {
    int current_fbo_id = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);
    if (current_fbo_id == this->m_fbo)
        unbind();

    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_depth_stencil);

    glDeleteTextures(1, &m_texture_primary);
    glDeleteTextures(1, &m_texture_secondary);

    if (!m_color_textures.empty()) {
        glDeleteTextures(static_cast<GLsizei>(m_color_textures.size()), m_color_textures.data());
        m_color_textures.clear();
    }
}
void RenderTarget::resize(int width, int height) {

    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_texture_primary);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, m_texture_secondary);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    for (GLuint texture : m_color_textures) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void RenderTarget::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void RenderTarget::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderTarget::swap_textures() {
    std::swap(m_texture_primary, m_texture_secondary);

    int current_fbo_id = 0;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_primary, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, current_fbo_id);
}
void RenderTarget::add_color_texture() {
    GLint current_fbo_id = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_color_textures.push_back(texture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_color_textures.size() - 1, GL_TEXTURE_2D, texture, 0);

    const size_t attachment_index = m_color_textures.size();


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_index, GL_TEXTURE_2D, texture, 0);
    std::vector<GLenum> buffers(m_color_textures.size() + 1);

    for (size_t i = 0; i < buffers.size(); i++)
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;

    glDrawBuffers(static_cast<GLsizei>(buffers.size()),buffers.data());
    glDrawBuffers(buffers.size(), buffers.data());
    glBindFramebuffer(GL_FRAMEBUFFER, current_fbo_id);
}
} // graphics
} // engine