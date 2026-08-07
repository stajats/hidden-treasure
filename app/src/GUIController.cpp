//
// Created by kaloyan on 7/21/26.
//

#include "engine/graphics/GraphicsController.hpp"
#include "../include/GUIController.hpp"
#include "../../engine/libs/glfw/include/GLFW/glfw3.h"
#include "MainController.hpp"
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
    ImGui::Text("Camera direction: (%f, %f, %f)", camera->Front.x, camera->Front.y, camera->Front.z);

    ImGui::SeparatorText("Lighting component switches:");
    ImGui::Checkbox("Enable ambient light", &(main->scene.ambient_light));
    ImGui::Checkbox("Enable directional light", &(main->scene.directional_light));
    ImGui::Checkbox("Enable point light", &(main->scene.point_light));
    ImGui::Checkbox("Enable spot light", &(main->scene.spot_light));

    ImGui::SeparatorText("Lighting color");
    ImGui::InputFloat3("Sun light", &(main->scene.sunLight.color).x, "%.3f");
    main->scene.sunLight.color = clamp(main->scene.sunLight.color, 0.0f, 1.0f);
    ImGui::InputFloat3("Lanthern light", &(main->scene.lanthern_color).x, "%.3f");
    main->scene.lanthern_color = clamp(main->scene.lanthern_color, 0.0f, 1.0f);
    for (int i = 0; i < main->scene.lantern_lights.size(); i++) {
        main->scene.lantern_lights[i].color = main->scene.lanthern_color;
    }
    ImGui::InputFloat3("Skull light", &(main->scene.skull_color).x, "%.3f");
    main->scene.skull_color = clamp(main->scene.skull_color, 0.0f, 1.0f);
    for (int i = 0; i < main->scene.flame_lights.size(); i++) {
        main->scene.flame_lights[i].color = main->scene.skull_color;
    }
    ImGui::InputFloat3("Spot light", &(main->scene.spot_light_color).x, "%.3f");
    main->scene.spot_light_color = clamp(main->scene.spot_light_color, 0.0f, 1.0f);
    ImGui::SeparatorText("Visual effects");
    ImGui::Checkbox("Enable animated water", &(main->scene.animated_water));
    ImGui::Checkbox("Enable point shadows", &(main->scene.point_shadows));

    ImGui::SeparatorText("Performance");
    ImGui::Text("FPS: %.1f (%.3f ms/frame)",
    ImGui::GetIO().Framerate,
    1000.0f / ImGui::GetIO().Framerate);

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