//
// Created by kaloyan on 8/4/26.
//

#include "../include/SkullController.hpp"

#include "../../engine/libs/glfw/include/GLFW/glfw3.h"
#include "MainController.hpp"

namespace app {

std::string_view app::SkullController::name() const {
    return "app::SkullController";
}
void SkullController::initialize() {
    this->set_enable(false);
}
void SkullController::update() {
    float time = glfwGetTime();
    if (time - this->skull_time > 2.0f * M_PI)
        this->set_enable(false);
}
void SkullController::draw() {
    auto main = engine::core::Controller::get<MainController>();
    if (is_enabled()) {
        for (auto object : main->scene.flame) {
            main->draw_basic(object.resource, object.transform, object.material, main->scene.sunLight);
        }
    }
}
} // app