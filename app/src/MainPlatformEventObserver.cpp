//
// Created by kaloyan on 8/4/26.
//

#include "../include/MainPlatformEventObserver.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "../../engine/libs/glfw/include/GLFW/glfw3.h"
#include "GUIController.hpp"
#include "SkullController.hpp"
#include "engine/platform/PlatformController.hpp"

namespace app {
void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}
void MainPlatformEventObserver::on_key(engine::platform::Key key) {

    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) return;
    auto skull_controller = engine::core::Controller::get<app::SkullController>();

    if (engine::platform::KeyId::KEY_E == key.id()) {
        skull_controller->set_enable(true);
        skull_controller->skull_time = glfwGetTime();
    }
}
} // app