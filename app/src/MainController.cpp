//
// Created by kaloyan on 7/20/26.
//

#include "../include/MainController.hpp"

#include "../../engine/test/app/include/app/GUIController.hpp"
#include "GUIController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}

void app::MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
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
void app::MainController::draw_boat() {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto boat = resources->model("boat");
    auto shader = resources->shader("basic");
    shader->use();
    glm::vec3 lightDir(-0.3f, -0.9f, -0.95f);
    shader->set_vec3("lightDir", lightDir);
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = scale(model, glm::vec3(0.3));
    shader->set_mat4("model", model);

    boat->draw(shader);
}
void app::MainController::draw() {

    draw_boat();
}
void app::MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;
    
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::FORWARD, platform->dt());
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::BACKWARD, platform->dt());
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::LEFT, platform->dt());
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::RIGHT, platform->dt());
    }

    auto observer = std::make_unique<MainPlatformEventObserver>();
}

void app::MainController::update() {
    update_camera();
}
