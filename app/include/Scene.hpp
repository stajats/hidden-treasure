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
    float radians;
    glm::vec3 rotation;
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
};
struct Resource
{
    std::string model_name;
    std::string shader_name;
};

struct Model {
    Resource model;
    Transform transform;
    Material material;
};
class Scene {
public:

    void load_scene();

    DirectionalLight sunLight;
    glm::vec3 lanthern_color;
    glm::vec3 spot_light_color;

    bool ambient_light;
    bool directional_light;
    bool point_light;
    bool spot_light;
    bool animated_water;

    std::vector<Model> objects;
    std::vector<Model> light_sources;
    std::vector<LightSource> lights;
};
}// namespace app

#endif//MATF_RG_PROJECT_SCENE_HPP
