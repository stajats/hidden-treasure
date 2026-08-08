//
// Created by kaloyan on 8/4/26.
//

#include "Scene.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "spdlog/spdlog.h"

namespace app {

void Scene::add_instances(const Resource& res, const Material& mat, const std::vector<InstanceData>& instances, std::vector<Model> &arr) {
    for (const auto& inst : instances) {
        arr.emplace_back(
            res,
            Transform(inst.pos, inst.angle, glm::normalize(inst.axis), inst.scale),
            mat
        );
    }
}
void Scene::load_scene() {

    this->objects.clear();
    this->lantern.clear();
    this->lantern_lights.clear();
    this->flame.clear();
    this->flame_lights.clear();

    enable_bloom = true;
    ambient_light = true;
    directional_light = true;
    point_light = true;
    spot_light = true;
    animated_water = true;
    point_shadows = true;
    pulsating_light = true;

    spot_light_color = glm::vec3(0.5f, 0.5f, 0.5f);
    lanthern_color = glm::vec3(0.851, 0.114, 0.039);
    skull_color = glm::vec3(0.0f, 1.0f, 0.0f);
    sun_light = DirectionalLight(
        -glm::normalize(glm::vec3(-0.95f, -0.16f, 0.3f)),
        glm::vec3(1.0, 0.6549, 0.149)
    );

    Material mat_stone(0.05f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f);
    Material mat_wood(0.10f, glm::vec3(0.25f, 0.22f, 0.2f), 10.0f);
    Material mat_metal(0.15f, glm::vec3(0.4f, 0.4f, 0.4f), 32.0f);
    Material mat_water(0.10f, glm::vec3(0.8f, 0.9f, 1.0f), 128.0f);


    auto add_instances = [&](const Resource& res, const Material& mat, const std::vector<InstanceData>& instances, std::vector<Model> &arr) {
        for (const auto& inst : instances) {
            arr.emplace_back(
                res,
                Transform(inst.pos, inst.angle, glm::normalize(inst.axis), inst.scale),
                mat
            );
        }
    };

    this->objects.emplace_back(
        Resource("boat", "basic"),
        Transform(glm::vec3(-1.0f, 0.0f, 17.3f), 31.0f, glm::vec3(-2.14, 4.01, -2.05), glm::vec3(0.3f)),
        mat_wood
    );

    this->objects.emplace_back(
        Resource("island", "basic"),
        Transform(glm::vec3(0.0f, -0.21f, 20.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.09f)),
        mat_stone
    );

    this->objects.emplace_back(
        Resource("chest", "basic"),
        Transform(glm::vec3(-1.6f, 1.14f, 20.5f), -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.2f)),
        mat_wood
    );

    this->objects.emplace_back(
        Resource("ship", "basic"),
        Transform(glm::vec3(5.0f, 0.5f, 2.0f), -240.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f)),
        mat_wood
    );

    this->objects.emplace_back(
        Resource("water", "water"),
        Transform(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(500.0f)),
        mat_water
    );

    add_instances(Resource("rock", "basic"), mat_stone, {
        { glm::vec3( 13.00f,  0.0f, 44.50f),  10.0f, glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3(-25.83f, -1.0f, 23.48f), -25.0f, glm::vec3( 1.0f, 0.5f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3(  0.24f, -3.0f, 30.69f), 120.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3( 18.81f, -0.3f, 20.93f),  10.0f, glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) },
        { glm::vec3( 15.12f, -0.7f,  2.20f),  30.0f, glm::vec3( 0.0f, 1.0f, 1.0f), glm::vec3(15.0f, 25.0f, 15.0f) }
    }, objects);

    add_instances(Resource("big_rock", "basic"), mat_stone, {
        { glm::vec3( 11.80f, -1.0f, 28.40f),  15.0f, glm::vec3(0.00f, 0.0f, 1.00f), glm::vec3(10.0f, 20.0f, 10.0f) },
        { glm::vec3(  0.00f, -0.5f, 40.50f),  60.0f, glm::vec3(0.01f, 1.0f, 0.02f), glm::vec3(10.0f, 20.0f, 10.0f) },
        { glm::vec3(-15.60f, -5.0f, 31.80f), -10.0f, glm::vec3(0.00f, 0.0f, 1.00f), glm::vec3(10.0f, 20.0f, 10.0f) },
        { glm::vec3(-24.20f, -2.0f, 11.00f),   5.0f, glm::vec3(0.00f, 0.0f, 1.00f), glm::vec3(10.0f, 20.0f, 10.0f) }
    }, objects);

    add_instances(Resource("small_rock", "basic"), mat_stone, {
        { glm::vec3(  4.00f, -0.10f, 37.00f),  30.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( 17.70f, -0.10f, 17.90f),  60.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( -3.80f, -0.10f, 34.70f),  75.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3(  4.20f, -0.25f, 22.10f), 130.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( 16.40f, -0.10f, 24.30f), 275.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3(-29.90f, -0.10f, 25.80f), 330.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3( -5.40f, -0.30f, 22.40f),  80.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) },
        { glm::vec3(-24.10f, -0.10f,  6.70f), 150.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f) }
    }, objects);

    add_instances(Resource("lantern", "basic"), mat_metal, {
        { glm::vec3(-2.78f, 0.92f, 19.33f),  30.0f, glm::vec3(-0.5f, 0.0f, 1.0f), glm::vec3(0.5f) },
        { glm::vec3( 0.40f, 1.04f, 20.00f),  60.0f, glm::vec3( 0.0f, 1.0f, 0.2f), glm::vec3(0.5f) },
        { glm::vec3(-1.79f, 0.55f, 18.03f),  35.0f, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3( 1.98f, 2.44f, -1.33f), 130.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3( 7.86f, 2.03f,  5.70f), 275.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3( 5.24f, 1.52f, 0.035f), 330.0f, glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3(-3.84f, 0.24f, 21.41f),  80.0f, glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.5f) },
        { glm::vec3(-0.69f, 0.91f, 21.80f),  55.0f, glm::vec3( 0.5f, 1.0f, 0.0f), glm::vec3(0.5f) }
    }, lantern);

    add_instances(Resource("flame", "basic"), mat_metal, {
        {glm::vec3(-1.941358f, 1.751783f, 20.432997f), 208.294342f, glm::vec3(-0.684907f, -0.419964f, 0.802914f), glm::vec3(0.025f) },
        {glm::vec3(-2.072591f, 1.710645f, 20.281853f), 143.233948f, glm::vec3(-1.378606f, -0.681398f, 0.062815f), glm::vec3(0.03f) },
    }, flame);
    for (int i = 0; i < lantern.size(); i++) {
        glm::vec3 offset(0.0f, 0.2f, 0.0f); // inside lantern

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(lantern[i].transform.angle), lantern[i].transform.axis);
        glm::vec3 worldOffset = glm::vec3(rotationMatrix * glm::vec4(offset, 0.0f));
        glm::vec3 lightCenterPos = lantern[i].transform.translation + worldOffset;

        lantern_lights.push_back(LightSource(lanthern_color, lightCenterPos));
    }
    flame_lights.push_back(LightSource(skull_color, glm::vec3(-1.941358f, 1.751783f, 20.432997f)));
    flame_lights.push_back(LightSource(skull_color, glm::vec3(-2.072591f, 1.710645f, 20.281853f)));

    model_names = std::vector<std::string>(objects.size());
    translation = std::vector<glm::vec3>(objects.size());
    angle = std::vector<float>(objects.size());
    axis = std::vector<glm::vec3>(objects.size());
    scale = std::vector<glm::vec3>(objects.size());

    for (int i = 0; i < objects.size(); i++) {
        model_names[i] = objects[i].resource.model;
        translation[i] = objects[i].transform.translation;
        angle[i] = objects[i].transform.angle;
        axis[i] = objects[i].transform.axis;
        scale[i] = objects[i].transform.scale;
    }
}
} // app