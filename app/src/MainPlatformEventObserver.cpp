//
// Created by kaloyan on 8/4/26.
//

#include "../include/MainPlatformEventObserver.hpp"
#include "GUIController.hpp"
#include "engine/graphics/GraphicsController.hpp"

namespace app {
void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}
} // app