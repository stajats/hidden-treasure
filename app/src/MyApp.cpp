//
// Created by kaloyan on 7/20/26.
//

#include "MyApp.hpp"
#include "GUIController.hpp"
#include "MainController.hpp"
#include "SkullController.hpp"
#include "spdlog/spdlog.h"

void MyApp::app_setup() {
    spdlog::info("App setup completed!");
    auto main_controller = register_controller<app::MainController>();
    auto gui_controller = register_controller<app::GUIController>();
    auto skull_controller = register_controller<app::SkullController>();

    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(main_controller);
    skull_controller->after(main_controller);
}