//
// Created by kaloyan on 8/5/26.
//

#ifndef MATF_RG_PROJECT_POINTSHADOWMAP_HPP
#define MATF_RG_PROJECT_POINTSHADOWMAP_HPP
#include <cstdint>

namespace engine {
namespace graphics {
class GraphicsController;
}
}

namespace engine {
namespace resources {

/**
 *  @class PointShadowMap
 *  @brief Stores data needed for drawing point shadow maps
 */
class PointShadowMap {

    friend class engine::graphics::GraphicsController;

public:
    /**
    * @brief Returns the OpenGL ID of the point shadow map.
    * @returns The OpenGL ID of the point shadow map.
    */
    uint32_t fbo() const {
        return m_fbo;
    }

    /**
    * @brief Returns the OpenGL ID of the point shadow map texture.
    * @returns The OpenGL ID of the point shadow map texture.
    */
    uint32_t cubemap() const {
        return m_cubemap_id;
    }
    uint32_t size() const {
        return m_size;
    }
    /**
    * @brief Destroys the point shadow map object in the OpenGL context.
    */
    void destroy();

private:
    PointShadowMap() = default;

    uint32_t m_fbo{0};
    uint32_t m_cubemap_id{0};
    uint32_t m_size{0};

    /**
    * @brief Constructs a PointShadowMap object.
    * @param fbo The OpenGL ID of the point shadow map.
    * @param cubemap_id The OpenGL ID of the point shadow map texture.
    */
    PointShadowMap(uint32_t fbo, uint32_t cubemap_id, uint32_t m_size)
        : m_fbo(fbo)
        , m_cubemap_id(cubemap_id)
        , m_size(m_size) {
    }
};

}// namespace resources
}// namespace engine

#endif // MATF_RG_PROJECT_POINTSHADOWMAP_HPP
