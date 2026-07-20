//
// Created by kaloyan on 7/20/26.
//

#include "../include/MyApp.hpp"

#include "MainController.hpp"
#include "spdlog/spdlog.h"

void MyApp::app_setup() {
    spdlog::info("App setup completed!");
    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}