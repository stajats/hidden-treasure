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

    //auto sunLight = DirectionalLight(glm::normalize(glm::vec3(-0.3f, -0.9f, -0.95f)), glm::vec3(1.0f, 1.0f, 1.0f));

   auto sunLight = DirectionalLight(glm::normalize(glm::vec3(-0.95f, -0.16f, 0.3f)), glm::vec3(1.0f, 1.0f, 1.0f));
    draw_basic(Resource("boat", "basic"),
               Transform(glm::vec3(0.0f, 0.0f, -3.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f)),
               Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
               sunLight);
    for (int i = 0; i < 8; i++) {
        std::vector<glm::vec3> translations = {
            glm::vec3(-5.0f,0.0f, 10.5f),
            glm::vec3( 3.83f, 0.0f,  9.48f),
            glm::vec3( 9.24f, 0.0f, 15.69f),
            glm::vec3(10.81f, 0.0f, 23.93f),
            glm::vec3( 5.30f, 0.0f, 28.48f),
            glm::vec3(-4.12f, 0.0f, 30.20f),
            glm::vec3(-9.01f, 0.0f, 25.20f),
            glm::vec3(-10.53f, 0.0f, 17.18f),
        };
        draw_basic(Resource("rock", "basic"),
                   Transform(translations[i], -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(15.0f, 25.0f, 15.0f)),
                   Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
                   sunLight);
    }
    for (int i = 0; i < 8; i++) {
        std::vector<glm::vec3> translations = {
            glm::vec3(-10.80f, 0.0f,  0.40f ),
            glm::vec3(  7.20f, 0.0f, -1.80f ),
            glm::vec3( 18.90f, 0.0f, 11.10f ),
            glm::vec3( 21.40f, 0.0f, 27.60f ),
            glm::vec3( 11.10f, 0.0f, 37.10f ),
            glm::vec3( -8.90f, 0.0f, 40.50f ),
            glm::vec3(-18.60f, 0.0f, 30.80f ),
            glm::vec3(-21.20f, 0.0f, 14.10f ),
        };
        draw_basic(Resource("rock1", "basic"),
                   Transform(translations[i], -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(15.0f, 25.0f, 15.0f)),
                   Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
                   sunLight);
    }
    for (int i = 0; i < 8; i++) {
        std::vector<glm::vec3> translations = {
            glm::vec3( -2.35f, -0.1f, 15.60f ),
            glm::vec3(  1.70f, -0.1f, 14.90f ),
            glm::vec3(  4.80f, -0.1f, 17.70f ),
            glm::vec3(  5.20f, -0.1f, 22.10f ),
            glm::vec3(  2.40f, -0.1f, 24.30f ),
            glm::vec3( -1.90f, -0.1f, 24.80f ),
            glm::vec3( -4.40f, -0.1f, 22.40f ),
            glm::vec3( -5.10f, -0.1f, 18.70f ),
        };
        draw_basic(Resource("rocks", "basic"),
                   Transform(translations[i], -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f, 4.0f, 4.0f)),
                   Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
                   sunLight);
    }
    draw_basic(Resource("island", "basic"),
              Transform(glm::vec3(0.0f, -0.21f, 20.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.09f)),
              Material(0.25f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f),
              sunLight);
    draw_basic(Resource("chest", "basic"),
              Transform(glm::vec3(-1.6f, 1.15f, 20.5f), -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.2f)),
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
