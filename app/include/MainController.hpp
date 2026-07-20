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

public:
    std::string_view name() const override;
};

}// namespace app


#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
