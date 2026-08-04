//
// Created by kaloyan on 7/21/26.
//

#include "../include/GUIController.hpp"

#include "../../engine/libs/glfw/include/GLFW/glfw3.h"
#include "MainController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform//PlatformController.hpp"
#include "imgui.h"

namespace app {

float a[3];
void GUIController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    set_enable(false);
    auto window = platform->window();
    glfwSetInputMode(window->handle_(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto main = engine::core::Controller::get<MainController>();

    auto camera = graphics->camera();

    graphics->begin_gui();

    ImGui::SeparatorText("Camera info");
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Front.x, camera->Front.y, camera->Front.z);

    ImGui::SeparatorText("Lighting component switches:");
    ImGui::Checkbox("Enable ambient light", &(main->ambient_light));
    ImGui::Checkbox("Enable directional light", &(main->directional_light));
    ImGui::Checkbox("Enable point light", &(main->point_light));
    ImGui::Checkbox("Enable spot light", &(main->spot_light));

    ImGui::SeparatorText("Lighting color");
    ImGui::InputFloat3("Sun light", &(main->sun_light_color).x, "%.3f");
    main->sun_light_color = clamp(main->sun_light_color, 0.0f, 1.0f);
    ImGui::InputFloat3("Lanthern light", &(main->lanthern_color).x, "%.3f");
    main->lanthern_color = clamp(main->lanthern_color, 0.0f, 1.0f);
    for (int i = 0; i < main->lights.size(); i++) {
        main->lights[i].color = main->lanthern_color;
    }
    ImGui::InputFloat3("Spot light", &(main->spot_light_color).x, "%.3f");
    main->spot_light_color = clamp(main->spot_light_color, 0.0f, 1.0f);
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