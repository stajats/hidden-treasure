//
// Created by kaloyan on 8/4/26.
//

#ifndef MATF_RG_PROJECT_SCENE_HPP
#define MATF_RG_PROJECT_SCENE_HPP

#include "glm/vec3.hpp"
#include <string>
#include <vector>

namespace app {

struct Transform
{
    glm::vec3 translation;
    float angle;
    glm::vec3 axis;
    glm::vec3 scale;
};

struct Material
{
    float ambient;
    glm::vec3 specular;
    float shininess;
};

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 color;
};
struct LightSource {
    glm::vec3 color;
    glm::vec3 position;
    uint32_t shadow_map_id;
};
struct Resource
{
    std::string model;
    std::string shader_name;
};

struct Model {
    Resource resource;
    Transform transform;
    Material material;
};
class Scene {
public:

    void load_scene();

    DirectionalLight sunLight;
    glm::vec3 lanthern_color;
    glm::vec3 spot_light_color;
    glm::vec3 skull_color;

    bool ambient_light;
    bool directional_light;
    bool point_light;
    bool spot_light;
    bool animated_water;
    bool point_shadows;
    bool enable_bloom;
    bool pulsating_light;

    std::vector<Model> objects;
    std::vector<Model> lantern;
    std::vector<LightSource> lantern_lights;
    std::vector<Model> flame;
    std::vector<LightSource> flame_lights;
};
}// namespace app

#endif//MATF_RG_PROJECT_SCENE_HPP
