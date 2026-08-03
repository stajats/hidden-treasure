//
// Created by kaloyan on 7/21/26.
//

#include "../include/GUIController.hpp"

#include "../../engine/libs/glfw/include/GLFW/glfw3.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform//PlatformController.hpp"
#include "imgui.h"

namespace app {

void GUIController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    set_enable(false);
    auto window = platform->window();
    glfwSetInputMode(window->handle_(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();

    graphics->begin_gui();

    ImGui::Begin("Camera info");
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Front.x, camera->Front.y, camera->Front.z);


    ImGui::End();

    graphics->end_gui();
}

void GUIController::poll_events() {

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_Q).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
        auto window = platform->window();
        if (is_enabled() == true) {
            glfwSetInputMode(window->handle_(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window->handle_(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}
std::string_view app::GUIController::name() const {
    return "app::GUIController";
}
}// namespace app