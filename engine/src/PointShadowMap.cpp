//
// Created by kaloyan on 8/5/26.
//

#include "engine/resources/PointShadowMap.hpp"
#include "glad/glad.h"

namespace engine {
namespace resources {
void PointShadowMap::destroy()
{
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }

    if (m_cubemap_id != 0) {
        glDeleteTextures(1, &m_cubemap_id);
        m_cubemap_id = 0;
    }
}

} // resources
} // engine