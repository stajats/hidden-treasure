//
// Created by kaloyan on 8/4/26.
//

#ifndef MATF_RG_PROJECT_MAINPLATFORMEVENTOBSERVER_HPP
#define MATF_RG_PROJECT_MAINPLATFORMEVENTOBSERVER_HPP

#include "engine/platform/Input.hpp"
#include "engine/platform/PlatformEventObserver.hpp"

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
    void on_key(engine::platform::Key key) override;
};
}

#endif//MATF_RG_PROJECT_MAINPLATFORMEVENTOBSERVER_HPP
