//
// Created by kaloyan on 7/20/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include "engine/core/Controller.hpp"
#include "glm/vec3.hpp"

namespace app {
class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw() override;
    void update_camera();
    void begin_draw() override;
    void end_draw() override;
    void update() override;

public:
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

    struct Resource
    {
        std::string model_name;
        std::string shader_name;
    };
    std::string_view name() const override;
    void draw_basic(Resource r, Transform t, Material m, DirectionalLight dl);
    void draw_boat();
    void draw_skybox();
    void draw_chest();
    void draw_rock();
    void draw_island();
    void draw_water();
};

}// namespace app


#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
