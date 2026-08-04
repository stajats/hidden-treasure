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
void app::MainController::load_scene() {

    this->objects.clear();
    this->light_sources.clear();

    auto sunLight = DirectionalLight(
        glm::normalize(glm::vec3(-0.95f, -0.16f, 0.3f)),
        glm::vec3(1.0, 0.6549, 0.149) / 3.0f
    );

    Material matStone(0.15f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f);
    Material matWood(0.20f, glm::vec3(0.25f, 0.22f, 0.2f), 10.0f);
    Material matMetal(0.25f, glm::vec3(0.4f, 0.4f, 0.4f), 32.0f);
    Material matWater(0.10f, glm::vec3(0.8f, 0.9f, 1.0f), 128.0f);

    struct InstanceData {
        glm::vec3 pos;
        float angle;
        glm::vec3 axis;
        glm::vec3 scale;
    };

    auto add_instances = [&](const Resource& res, const Material& mat, const std::vector<InstanceData>& instances) {
        for (const auto& inst : instances) {
            this->objects.emplace_back(
                res,
                Transform(inst.pos, inst.angle, glm::normalize(inst.axis), inst.scale),
                mat,
                sunLight
            );
        }
    };
    auto add_light_instances = [&](const Resource& res, const Material& mat, const std::vector<InstanceData>& instances) {
        for (const auto& inst : instances) {
            this->light_sources.emplace_back(
                res,
                Transform(inst.pos, inst.angle, glm::normalize(inst.axis), inst.scale),
                mat,
                sunLight
            );
        }
    };


    this->objects.emplace_back(
        Resource("boat", "basic"),
        Transform(glm::vec3(-1.0f, 0.0f, 17.3f), 31.0f, glm::vec3(-2.14, 4.01, -2.05), glm::vec3(0.3f)),
        matWood,
        sunLight
    );

    this->objects.emplace_back(
        Resource("island", "basic"),
        Transform(glm::vec3(0.0f, -0.21f, 20.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.09f)),
        matStone,
        sunLight
    );

    this->objects.emplace_back(
        Resource("chest", "basic"),
        Transform(glm::vec3(-1.6f, 1.14f, 20.5f), -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.2f)),
        matWood,
        sunLight
    );

    this->objects.emplace_back(
        Resource("ship", "basic"),
        Transform(glm::vec3(5.0f, 0.5f, 2.0f), -240.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f)),
        matWood,
        sunLight
    );

    this->objects.emplace_back(
        Resource("water", "water"),
        Transform(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(500.0f)),
        matWater,
        sunLight
    );

    add_instances(Resource("rock", "basic"), matStone, {
        { glm::vec3( 13.00f,  0.0f, 44.50f),  10.0f, glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3(-25.83f, -1.0f, 23.48f), -25.0f, glm::vec3( 1.0f, 0.5f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3(  0.24f, -3.0f, 30.69f), 120.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3( 18.81f, -0.3f, 20.93f),  10.0f, glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3( 15.12f, -0.7f,  2.20f),  30.0f, glm::vec3( 0.0f, 1.0f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) }
    });

    add_instances(Resource("rock1", "basic"), matStone, {
        { glm::vec3( 11.80f, -1.0f, 28.40f),  15.0f, glm::vec3(0.00f, 0.0f, 1.00f), glm::vec3(10.0f, 20.0f, 10.0f) },
        { glm::vec3(  0.00f, -0.5f, 40.50f),  60.0f, glm::vec3(0.01f, 1.0f, 0.02f), glm::vec3(10.0f, 20.0f, 10.0f) },
        { glm::vec3(-15.60f, -5.0f, 31.80f), -10.0f, glm::vec3(0.00f, 0.0f, 1.00f), glm::vec3(10.0f, 20.0f, 10.0f) },
        { glm::vec3(-24.20f, -2.0f, 11.00f),   5.0f, glm::vec3(0.00f, 0.0f, 1.00f), glm::vec3(10.0f, 20.0f, 10.0f) }
    });

    add_instances(Resource("rocks", "basic"), matStone, {
        { glm::vec3(  4.00f, -0.10f, 37.00f),  30.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( 17.70f, -0.10f, 17.90f),  60.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( -3.80f, -0.10f, 34.70f),  75.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3(  4.20f, -0.25f, 22.10f), 130.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( 16.40f, -0.10f, 24.30f), 275.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3(-29.90f, -0.10f, 25.80f), 330.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( -5.40f, -0.30f, 22.40f),  80.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3(-24.10f, -0.10f,  6.70f), 150.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) }
    });

    add_light_instances(Resource("lantern", "basic"), matMetal, {
        { glm::vec3(-2.78f, 0.92f, 19.33f),  30.0f, glm::vec3(-0.5f, 0.0f, 1.0f), glm::vec3(0.5f) },
        { glm::vec3( 0.40f, 1.04f, 20.00f),  60.0f, glm::vec3( 0.0f, 1.0f, 0.2f), glm::vec3(0.5f) },
        { glm::vec3(-1.79f, 0.55f, 18.03f),  35.0f, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3( 1.98f, 2.44f, -1.33f), 130.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3( 7.86f, 2.03f,  5.70f), 275.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3( 5.24f, 1.52f, 0.035f), 330.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3(-3.84f, 0.24f, 21.41f),  80.0f, glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3(-0.69f, 0.91f, 21.80f),  55.0f, glm::vec3( 0.5f, 1.0f, 0.0f), glm::vec3(0.5f) }
    });
    for (int i = 0; i < light_sources.size(); i++) {
        lights.push_back(LightSource(glm::vec3(0.851, 0.114, 0.039), glm::vec3(0.0f, 0.2f, 0.0f)));
    }
}

void app::MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());

    ambient_light = true;
    directional_light = true;
    point_light = true;
    spot_light = true;
    load_scene();
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

    shader->set_int("num_of_light_sources", this->light_sources.size());
    for (int i = 0; i < this->light_sources.size(); i++) {
        glm::vec3 localOffset = lights[i].position;

        glm::mat4 rotationMatrix = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(light_sources[i].transform.radians),
            light_sources[i].transform.rotation
        );

        glm::vec3 worldOffset = glm::vec3(rotationMatrix * glm::vec4(localOffset, 0.0f));

        // 3. Final light position in world space
        glm::vec3 lightCenterPos = light_sources[i].transform.translation + worldOffset;

        shader->set_vec3("lights[" + std::to_string(i) + "].position", lightCenterPos);
        shader->set_vec3("lights[" + std::to_string(i) + "].color", lights[i].color);

        shader->set_float("lights[" + std::to_string(i) + "].constant",  1.0f);
        shader->set_float("lights[" + std::to_string(i) + "].linear",    0.09f);
        shader->set_float("lights[" + std::to_string(i) + "].quadratic", 0.032f);
    }
    shader->set_float("light.constant",  1.0f);
    shader->set_float("light.linear",    0.09f);
    shader->set_float("light.quadratic", 0.032f);
    shader->set_vec3("light.position",  graphics->camera()->Position);
    shader->set_vec3("light.direction", graphics->camera()->Front);

    shader->set_bool("enableAmbient", ambient_light);
    shader->set_bool("enableDirectional", directional_light);
    shader->set_bool("enablePoint", point_light);
    shader->set_bool("enableSpot", spot_light);

    shader->set_float("light.cutOff",   glm::cos(glm::radians(12.5f)));
    shader->set_float("light.outerCutOff", glm::cos(glm::radians(90.5f)));
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

    for (auto object: this->objects) {
        draw_basic(object.model, object.transform, object.material, object.directional_light);
    }

    for (auto object: this->light_sources) {
        draw_basic(object.model, object.transform, object.material, object.directional_light);
    }
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
