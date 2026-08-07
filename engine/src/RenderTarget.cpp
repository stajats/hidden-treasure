//
// Created by kaloyan on 8/7/26.
//

#include "../include/engine/graphics/RenderTarget.hpp"
#include "engine/util/Errors.hpp"

namespace engine {
namespace graphics {

RenderTarget::RenderTarget(int width, int height) {

    GLint currentFboId = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFboId);

    m_width = width;
    m_height = height;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    m_color_textures.resize(1);
    glGenTextures(1,m_color_textures.data());
    glBindTexture(GL_TEXTURE_2D,m_color_textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, m_color_textures[0], 0);

    std::vector<GLenum> attachments;
    attachments.push_back(GL_COLOR_ATTACHMENT0 + 0);

    glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());
    glGenRenderbuffers(1, &m_depth_stencil);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_stencil);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw util::Error("RenderTarget incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, currentFboId);
}

RenderTarget::~RenderTarget() {

    GLint currentFboId = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFboId);
    if (currentFboId == this->m_fbo)
        unbind();

    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_depth_stencil);

    if (!m_color_textures.empty()) {
        glDeleteTextures(static_cast<GLsizei>(m_color_textures.size()), m_color_textures.data());
        m_color_textures.clear();
    }
}

void RenderTarget::resize(int width, int height) {

    m_width = width;
    m_height = height;

    for (GLuint texture : m_color_textures) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void RenderTarget::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void RenderTarget::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::addColorTexture() {
    GLint currentFboId = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFboId);

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

    std::vector<GLenum> buffers;

    for(size_t i = 0; i < m_color_textures.size(); i++)
        buffers.push_back(GL_COLOR_ATTACHMENT0 + i);

    glDrawBuffers(buffers.size(), buffers.data());
    glBindFramebuffer(GL_FRAMEBUFFER, currentFboId);
}
} // graphics
} // engine