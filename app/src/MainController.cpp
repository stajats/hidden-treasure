//
// Created by kaloyan on 7/20/26.
//

#include "../include/MainController.hpp"

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

#include <linux/input-event-codes.h>

void app::MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
}
bool app::MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
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
