//
// Created by kaloyan on 7/20/26.
//

#include "../include/MyApp.hpp"

#include "GUIController.hpp"
#include "MainController.hpp"
#include "spdlog/spdlog.h"

void MyApp::app_setup() {
    spdlog::info("App setup completed!");
    auto main_controller = register_controller<app::MainController>();
    auto gui_controller = register_controller<app::GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(main_controller);
}