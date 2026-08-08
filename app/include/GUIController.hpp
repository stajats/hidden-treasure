//
// Created by kaloyan on 7/21/26.
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_HPP
#define MATF_RG_PROJECT_GUICONTROLLER_HPP

#include "engine/core/Controller.hpp"

namespace app {

class GUIController : public engine::core::Controller {
    void poll_events() override;

public:
    std::string_view name() const override;

private:
    void initialize() override;
    void draw() override;
};

}// namespace app

#endif//MATF_RG_PROJECT_GUICONTROLLER_HPP
