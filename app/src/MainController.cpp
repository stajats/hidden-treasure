//
// Created by kaloyan on 7/20/26.
//

#include "MainController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "GUIController.hpp"
#include "MainPlatformEventObserver.hpp"
#include "SkullController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"
#include "Scene.hpp"
#include <engine/resources/Skybox.hpp>
#include <imgui_impl_opengl3.h>

namespace app {
void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    scene.load_scene();

    graphics->generate_n_point_shadow_maps(scene.lantern_lights.size() + scene.flame_lights.size());
    graphics->add_color_texture();
    graphics->add_color_texture();
}
bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}
std::string_view app::MainController::name() const {
    return "app::MainController";
}
void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}
void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    if (scene.enable_bloom)
        graphics->bloom(1, "blur");
    graphics->finalize_draw("final");
    platform->swap_buffers();
}
void MainController::draw_skybox() {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("sunrise");
    auto shader = resources->shader("skybox");
    graphics->draw_skybox(shader, skybox);

}
void MainController::set_modifiers(engine::resources::Shader *shader) {
    // enable different effects
    shader->set_bool("enableAmbient", scene.ambient_light);
    shader->set_bool("enableDirectional", scene.directional_light);
    shader->set_bool("enablePoint", scene.point_light);
    shader->set_bool("enableSpot", scene.spot_light);
    shader->set_bool("enablePointShadow", scene.point_shadows);
    shader->set_float("enableAnimated", scene.animated_water);
}
void MainController::draw_basic(const Resource &r, const Transform &t, const Material &m, const DirectionalLight &dl) {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    auto mesh = resources->model(r.model);
    auto shader = resources->shader(r.shader_name);
    auto skull_controller = engine::core::Controller::get<SkullController>();

    float time = platform->frame_time().current;
    shader->use();
    set_modifiers(shader);
    // lantern lights
    int no_of_light_sources = this->scene.lantern_lights.size();
    for (int i = 0; i < this->scene.lantern_lights.size(); i++) {
        shader->set_vec3("lights[" + std::to_string(i) + "].position", scene.lantern_lights[i].position);
        if (scene.pulsating_light) {
            glm::vec3 light_color = scene.lantern_lights[i].color + glm::vec3(sin(time + i), sin(2 * time + i), sin(3 * time + i)) / 10.0f;
            shader->set_vec3("lights[" + std::to_string(i) + "].color", light_color);
        }
        else
            shader->set_vec3("lights[" + std::to_string(i) + "].color", scene.lantern_lights[i].color);

        shader->set_float("lights[" + std::to_string(i) + "].constant",  1.0f);
        shader->set_float("lights[" + std::to_string(i) + "].linear",    0.09f);
        shader->set_float("lights[" + std::to_string(i) + "].quadratic", 0.032f);
    }
    if (skull_controller->is_enabled()) {
        no_of_light_sources += this->scene.flame_lights.size();
        for (size_t i = this->scene.lantern_lights.size(); i < no_of_light_sources; i++) {
            shader->set_vec3("lights[" + std::to_string(i) + "].position", scene.flame_lights[i - scene.lantern_lights.size()].position);
            shader->set_vec3("lights[" + std::to_string(i) + "].color", scene.flame_lights[i - scene.lantern_lights.size()].color * std::sin((time - skull_controller->skull_time) / 2.0f));
            shader->set_float("lights[" + std::to_string(i) + "].constant",  1.0f);
            shader->set_float("lights[" + std::to_string(i) + "].linear",    0.07f);
            shader->set_float("lights[" + std::to_string(i) + "].quadratic", 0.002f);
        }
    }
    shader->set_int("num_of_light_sources", no_of_light_sources);

    if (scene.point_shadows) {
        for (size_t i = 0; i < scene.lantern_lights.size() + scene.flame_lights.size(); i++) {
            graphics->set_point_shadow_map(shader, i);
        }
    }
    shader->set_float("currentTime", time);
    // spotlight
    shader->set_float("light.cutOff",   glm::cos(glm::radians(12.5f)));
    shader->set_float("light.outerCutOff", glm::cos(glm::radians(90.5f)));
    shader->set_vec3("lightDir", dl.direction);
    shader->set_vec3("lightColor", dl.color);
    shader->set_float("light.constant",  1.0f);
    shader->set_float("light.linear",    0.09f);
    shader->set_float("light.quadratic", 0.032f);
    shader->set_vec3("light.position",  graphics->camera()->Position);
    shader->set_vec3("light.direction", graphics->camera()->Front);
    shader->set_vec3("light.color", scene.spot_light_color);
    // material
    shader->set_float("materialAmbient", m.ambient);
    shader->set_vec3("materialSpecular", m.specular);
    shader->set_float("materialShininess", m.shininess);

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    auto model = glm::mat4(1.0f);

    model = translate(model, t.translation);
    model = rotate(model, glm::radians(t.angle), t.axis);
    model = glm::scale(model, t.scale);

    shader->set_mat4("model", model);
    mesh->draw(shader);
}

void MainController::draw() {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    if (scene.point_shadows) {
        for (int i = 0; i < scene.lantern_lights.size(); i++)
            graphics->draw_point_shadow_map(scene.lantern_lights[i].position, graphics->point_shadow_map(i), scene.model_names, scene.translation, scene.angle, scene.axis, scene.scale);
        for (int i = scene.lantern_lights.size(); i < scene.flame_lights.size() + scene.lantern_lights.size(); i++) {
            graphics->draw_point_shadow_map(scene.flame_lights[i - scene.lantern_lights.size()].position, graphics->point_shadow_map(i), scene.model_names, scene.translation, scene.angle, scene.axis, scene.scale);
        }
    }

    for (auto &object: this->scene.objects) {
        draw_basic(object.resource, object.transform, object.material, scene.sun_light);
    }
    for (auto &object: this->scene.lantern) {
        draw_basic(object.resource, object.transform, object.material, scene.sun_light);
    }
    draw_skybox();
}
void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::FORWARD, platform->dt() * 5);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::BACKWARD, platform->dt() * 5);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::LEFT, platform->dt() * 5);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::RIGHT, platform->dt() * 5);
    }
    auto observer = std::make_unique<MainPlatformEventObserver>();
}

void MainController::update() {
    update_camera();
}
} //app