//
// Created by kaloyan on 8/4/26.
//

#include "../include/Scene.hpp"

#include "glm/ext/quaternion_geometric.hpp"

namespace app {

void Scene::load_scene() {

    this->objects.clear();
    this->light_sources.clear();


    ambient_light = true;
    directional_light = true;
    point_light = true;
    spot_light = true;
    animated_water = true;

    spot_light_color = glm::vec3(0.5f, 0.5f, 0.5f);
    lanthern_color = glm::vec3(0.851, 0.114, 0.039);

    sunLight = DirectionalLight(
        glm::normalize(glm::vec3(-0.95f, -0.16f, 0.3f)),
        glm::vec3(1.0, 0.6549, 0.149)
    );

    Material matStone(0.1f, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f);
    Material matWood(0.15f, glm::vec3(0.25f, 0.22f, 0.2f), 10.0f);
    Material matMetal(0.2f, glm::vec3(0.4f, 0.4f, 0.4f), 32.0f);
    Material matWater(0.15f, glm::vec3(0.8f, 0.9f, 1.0f), 128.0f);

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
                mat
            );
        }
    };
    auto add_light_instances = [&](const Resource& res, const Material& mat, const std::vector<InstanceData>& instances) {
        for (const auto& inst : instances) {
            this->light_sources.emplace_back(
                res,
                Transform(inst.pos, inst.angle, glm::normalize(inst.axis), inst.scale),
                mat
            );
        }
    };

    this->objects.emplace_back(
        Resource("boat", "basic"),
        Transform(glm::vec3(-1.0f, 0.0f, 17.3f), 31.0f, glm::vec3(-2.14, 4.01, -2.05), glm::vec3(0.3f)),
        matWood
    );

    this->objects.emplace_back(
        Resource("island", "basic"),
        Transform(glm::vec3(0.0f, -0.21f, 20.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.09f)),
        matStone
    );

    this->objects.emplace_back(
        Resource("chest", "basic"),
        Transform(glm::vec3(-1.6f, 1.14f, 20.5f), -220.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.2f)),
        matWood
    );

    this->objects.emplace_back(
        Resource("ship", "basic"),
        Transform(glm::vec3(5.0f, 0.5f, 2.0f), -240.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f)),
        matWood
    );

    this->objects.emplace_back(
        Resource("water", "water"),
        Transform(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(500.0f)),
        matWater
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
        lights.push_back(LightSource(lanthern_color, glm::vec3(0.0f, 0.2f, 0.0f)));
    }
}
} // app