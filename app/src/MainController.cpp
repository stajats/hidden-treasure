//
// Created by kaloyan on 7/20/26.
//

#include "../include/MainController.hpp"

#include "../../engine/test/app/include/app/GUIController.hpp"
#include "GUIController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}

void app::MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
}
bool app::MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}
std::string_view app::MainController::name() const {
    return "app::MainController";
}
void app::MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}
void app::MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
void app::MainController::draw_skybox() {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("sunrise");
    auto shader = resources->shader("skybox");
    graphics->draw_skybox(shader, skybox);
}
void app::MainController::draw_basic(Resource r, Transform t, Material m, DirectionalLight dl) {

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto mesh = resources->model(r.model_name);
    auto shader = resources->shader(r.shader_name);
    shader->use();

    shader->set_vec3("lightDir", dl.direction);
    shader->set_vec3("lightColor", dl.color);
    shader->set_float("materialAmbient", m.ambient);
    shader->set_vec3("materialSpecular", m.specular);
    shader->set_float("materialShininess", m.shininess);

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    glm::mat4 model = glm::mat4(1.0f);

    model = translate(model, t.translation);
    model = rotate(model, glm::radians(t.radians), t.rotation);
    model = scale(model, t.scale);

    shader->set_mat4("model", model);

    mesh->draw(shader);
}

void app::MainController::draw() {

    auto sunLight = DirectionalLight(glm::normalize(glm::vec3(-0.95f, -0.16f, 0.3f)), glm::vec3(0.5f, 0.5f, 0.5f));
    draw_basic(Resource("boat", "basic"),
               Transform(glm::vec3(-1.0f, 0.0f, 17.3f), 31.0f, glm::vec3(-2.14, 4.01, -2.05), glm::vec3(0.3f)),
               Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
               sunLight);
    for (int i = 0; i < 5; i++) {
        std::vector<glm::vec3> translations = {
            glm::vec3(13.0f,0.0f, 44.5f),
            glm::vec3( -25.83f, -1.0f,  23.48f),
            glm::vec3( 0.24f, -3.0f, 30.69f),
            glm::vec3(18.81f, -0.3f, 20.93f),
            glm::vec3(15.12f, -0.7f, 2.20f),
        };

        std::vector<glm::vec3> rotations = {
            glm::vec3( -1.0f, 0.0f, 1.0f ),
            glm::vec3( 1.0f, 0.5f, 1.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 0.0f, 1.0f ),
            glm::vec3( 0.0f, 1.0f, 1.0f ),
        };
        std::vector<float> radians = {
            10.0f,
            -25.0f,
            120.0f,
            10.0f,
            30.0f,
        };
        draw_basic(Resource("rock", "basic"),
                   Transform(translations[i], radians[i], rotations[i], glm::vec3(15.0f, 25.0f, 15.0f)),
                   Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
                   sunLight);
    }
    for (int i = 0; i < 4; i++) {
        std::vector<glm::vec3> translations = {
            glm::vec3(11.80f, -1.0f,  28.40f ),
            glm::vec3( 0.0f, -0.5f, 40.50f ),
            glm::vec3(-15.60f, -5.0f, 31.80f ),
            glm::vec3(-24.20f, -2.0f, 11.0f ),
        };
        std::vector<glm::vec3> rotations = {
            glm::vec3( 0.0f, 0.0f, 1.0f ),
            glm::vec3( 0.01f, 1.0f, 0.02f ),
            glm::vec3( 0.0f, 0.0f, 1.0f ),
            glm::vec3( 0.0f, 0.0f, 1.0f ),
        };
        std::vector<float> radians = {
            15.0f,
            60.0f,
            -10.0f,
            5.0f,
        };
        draw_basic(Resource("rock1", "basic"),
                   Transform(translations[i], radians[i], normalize(rotations[i]), glm::vec3(10.0f, 20.0f, 10.0f)),
                   Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
                   sunLight);
    }
    for (int i = 0; i < 8; i++) {
        std::vector<glm::vec3> translations = {
            glm::vec3( 4.0f, -0.1f, 37.0f ),
            glm::vec3(  17.70f, -0.1f, 17.90f ),
            glm::vec3(  -3.80f, -0.1f, 34.70f ),
            glm::vec3(  4.20f, -0.25f, 22.10f ),
            glm::vec3(  16.40f, -0.1f, 24.30f ),
            glm::vec3( -29.90f, -0.1f, 25.80f ),
            glm::vec3( -5.40f, -0.3f, 22.40f ),
            glm::vec3( -24.10f, -0.1f, 6.70f ),
        };
        std::vector<glm::vec3> rotations = {
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
            glm::vec3( 0.0f, 1.0f, 0.0f ),
        };
        std::vector<float> radians = {
            30.0f,
            60.0f,
            75.0f,
            130.0f,
            275.0f,
            330.0f,
            80.0f,
            150.0f,
        };
        draw_basic(Resource("rocks", "basic"),
                   Transform(translations[i], radians[i], rotations[i], glm::vec3(4.0f, 4.0f, 4.0f)),
                   Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
                   sunLight);
    }
    draw_basic(Resource("island", "basic"),
              Transform(glm::vec3(0.0f, -0.21f, 20.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.09f)),
              Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
              sunLight);
    draw_basic(Resource("chest", "basic"),
              Transform(glm::vec3(-1.6f, 1.14f, 20.5f), -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.2f)),
              Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
              sunLight);
    draw_basic(Resource("water", "water"),
              Transform(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(500.0f)),
              Material(0.25f, glm::vec3(0.8f, 0.8f, 0.8f), 64.0f),
              sunLight);
    draw_skybox();
}
void app::MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<app::GUIController>();
    if (gui_controller->is_enabled()) return;
    
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::FORWARD, platform->dt() * 3);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::BACKWARD, platform->dt() * 3);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::LEFT, platform->dt() * 3);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        graphics->camera()->move_camera(engine::graphics::Camera::Movement::RIGHT, platform->dt() * 3);
    }

    auto observer = std::make_unique<MainPlatformEventObserver>();
}

void app::MainController::update() {
    update_camera();
}
