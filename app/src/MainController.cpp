//
// Created by kaloyan on 7/20/26.
//

#include "../include/MainController.hpp"

#include "engine/platform/PlatformController.hpp"
#include "spdlog/spdlog.h"

#include <linux/input-event-codes.h>

void app::MainController::initialize() {}
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
