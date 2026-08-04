//
// Created by kaloyan on 8/4/26.
//

#ifndef MATF_RG_PROJECT_SKULLCONTROLLER_HPP
#define MATF_RG_PROJECT_SKULLCONTROLLER_HPP
#include "engine/core/Controller.hpp"

namespace app {

class SkullController : public engine::core::Controller {
public:
    std::string_view name() const override;
    float skull_time;

private:
    void update() override;
    void initialize() override;
    void draw() override;
};

}// namespace app

#endif//MATF_RG_PROJECT_SKULLCONTROLLER_HPP
