//
// Created by kaloyan on 7/20/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include "engine/core/Controller.hpp"

namespace app {
class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw() override;
    void begin_draw() override;
    void end_draw() override;

public:
    std::string_view name() const override;
    void draw_boat();
};

}// namespace app


#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
