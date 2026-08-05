//
// Created by kaloyan on 7/20/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP

#include "Scene.hpp"
#include "engine/core/Controller.hpp"
#include "engine/resources/Shader.hpp"

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

    Scene scene;
    std::string_view name() const override;
    void draw_basic(Resource r, Transform t, Material m, DirectionalLight dl);
    void draw_skybox();
};

}// namespace app


#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
