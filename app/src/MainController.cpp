//
// Created by kaloyan on 7/20/26.
//

#include "../include/MainController.hpp"

#include "../../engine/libs/glfw/include/GLFW/glfw3.h"
#include "../../engine/test/app/include/app/GUIController.hpp"
#include "GUIController.hpp"
#include "MainPlatformEventObserver.hpp"
#include "SkullController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

void app::MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    scene.load_scene();
}
bool app::MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}
std::string_view app::MainController::name() const {
    return "app::MainController";
}
void app::MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}
void app::MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
void app::MainController::draw_skybox() {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("sunrise");
    auto shader = resources->shader("skybox");
    graphics->draw_skybox(shader, skybox);
}
void app::MainController::draw_basic(Resource r, Transform t, Material m, DirectionalLight dl) {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto mesh = resources->model(r.model);
    auto shader = resources->shader(r.shader_name);
    auto skull_controller = engine::core::Controller::get<SkullController>();

    float time = glfwGetTime();
    shader->use();

    int no_of_light_sources = this->scene.lantern_lights.size();
    if (skull_controller->is_enabled())
        no_of_light_sources += this->scene.flame_lights.size();
    shader->set_int("num_of_light_sources", no_of_light_sources);

    // lantern lights
    for (int i = 0; i < this->scene.lantern_lights.size(); i++) {
        glm::vec3 localOffset = scene.lantern_lights[i].position;
        glm::mat4 rotationMatrix = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(scene.lantern[i].transform.angle),
            scene.lantern[i].transform.axis
        );
        glm::vec3 worldOffset = glm::vec3(rotationMatrix * glm::vec4(localOffset, 0.0f));
        glm::vec3 lightCenterPos = scene.lantern[i].transform.translation + worldOffset;

        shader->set_vec3("lights[" + std::to_string(i) + "].position", lightCenterPos);
        shader->set_vec3("lights[" + std::to_string(i) + "].color", scene.lantern_lights[i].color + glm::vec3(sin(time * 3 + i), sin(4 * time + i), sin( 5 * time + i)) / 20.0f);
        shader->set_vec3("lights[" + std::to_string(i) + "].color", scene.lantern_lights[i].color + glm::vec3(sin(time + i), sin(2 * time + i), sin(3 * time + i)) / 10.0f);

        shader->set_float("lights[" + std::to_string(i) + "].constant",  1.0f);
        shader->set_float("lights[" + std::to_string(i) + "].linear",    0.09f);
        shader->set_float("lights[" + std::to_string(i) + "].quadratic", 0.032f);
    }
    //skull lights
    if (skull_controller->is_enabled()) {
        for (int i = this->scene.lantern_lights.size(); i < no_of_light_sources; i++) {
            shader->set_vec3("lights[" + std::to_string(i) + "].position", scene.flame_lights[i - scene.lantern_lights.size()].position);
            shader->set_vec3("lights[" + std::to_string(i) + "].color", scene.flame_lights[i - scene.lantern_lights.size()].color * (float) (sin((time - skull_controller->skull_time) / 2.0f)));
            shader->set_float("lights[" + std::to_string(i) + "].constant",  1.0f);
            shader->set_float("lights[" + std::to_string(i) + "].linear",    0.09f);
            shader->set_float("lights[" + std::to_string(i) + "].quadratic", 0.032f);
        }
    }
    shader->set_float("currentTime", time);
    // enable different effects
    shader->set_bool("enableAmbient", scene.ambient_light);
    shader->set_bool("enableDirectional", scene.directional_light);
    shader->set_bool("enablePoint", scene.point_light);
    shader->set_bool("enableSpot", scene.spot_light);
    shader->set_float("enableAnimated", scene.animated_water);
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

    glm::mat4 model = glm::mat4(1.0f);

    model = translate(model, t.translation);
    model = rotate(model, glm::radians(t.angle), t.axis);
    model = scale(model, t.scale);

    shader->set_mat4("model", model);

    mesh->draw(shader);
}

void app::MainController::draw() {

    for (auto object: this->scene.objects) {
        draw_basic(object.resource, object.transform, object.material, scene.sunLight);
    }
    for (auto object: this->scene.lantern) {
        draw_basic(object.resource, object.transform, object.material, scene.sunLight);
    }
    draw_skybox();
}
void app::MainController::update_camera() {
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

void app::MainController::update() {
    update_camera();
}
