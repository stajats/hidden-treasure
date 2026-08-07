
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <engine/graphics/GraphicsController.hpp>
#include <engine/resources/ResourcesController.hpp>

#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Skybox.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>


namespace engine::resources {
class PointShadowMap;
}


namespace engine::graphics {

void GraphicsController::initialize() {
    const int opengl_initialized = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    RG_GUARANTEE(opengl_initialized, "OpenGL failed to init!");

    auto platform = engine::core::Controller::get<platform::PlatformController>();
    auto handle = platform->window()->handle_();
    m_perspective_params.FOV = glm::radians(m_camera.Zoom);
    m_perspective_params.Width = static_cast<float>(platform->window()->width());
    m_perspective_params.Height = static_cast<float>(platform->window()->height());
    m_perspective_params.Near = 0.1f;
    m_perspective_params.Far = 100.f;
    m_ortho_params.Bottom = 0.0f;
    m_ortho_params.Top = static_cast<float>(platform->window()->height());
    m_ortho_params.Left = 0.0f;
    m_ortho_params.Right = static_cast<float>(platform->window()->width());
    m_ortho_params.Near = 0.1f;
    m_ortho_params.Far = 100.0f;

    m_render_target = new RenderTarget(platform->window()->width(), platform->window()->height());
    m_render_target_secondary = new RenderTarget(platform->window()->width(), platform->window()->height());
    m_bloom = nullptr;

    platform->register_platform_event_observer(std::make_unique<GraphicsPlatformEventObserver>(this));
    CHECKED_GL_CALL(glViewport, 0, 0, platform->window()->width(), platform->window()->height());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    RG_GUARANTEE(ImGui_ImplGlfw_InitForOpenGL(handle, true), "ImGUI failed to initialize for OpenGL");
    RG_GUARANTEE(ImGui_ImplOpenGL3_Init("#version 330 core"), "ImGUI failed to initialize for OpenGL");
}
void GraphicsController::add_color_texture() const {
    m_render_target->addColorTexture();
    m_render_target_secondary->addColorTexture();
}
void GraphicsController::finalize_draw() {

    auto platform =core::Controller::get<platform::PlatformController>();
    auto resources =core::Controller::get<resources::ResourcesController>();

    int width = platform->window()->width();
    int height = platform->window()->height();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    auto* shader = resources->shader("final");
    shader->use();
    shader->set_int("scene", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_render_target->texture(0));

    glDisable(GL_DEPTH_TEST);
    render_quad();
    glEnable(GL_DEPTH_TEST);

    m_render_target->bind();
}

void GraphicsController::render_quad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
void GraphicsController::bloom(int index) {
    auto platform = core::Controller::get<platform::PlatformController>();
    auto resources = engine::core::Controller::get<resources::ResourcesController>();

    int width = platform->window()->width();
    int height = platform->window()->height();

    if (m_bloom == nullptr)
        m_bloom = new Bloom(width, height);
    else
        m_bloom->resize(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, m_bloom->fbo(0));
    glViewport(0, 0, platform->window()->width(), platform->window()->height());

    bool horizontal = true, first_iteration = true;
    unsigned int amount = 30;

    auto *blur = resources->shader("blur");
    blur->use();
    blur->set_int("image", 0);

    for (int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,horizontal ? m_bloom->fbo(0) : m_bloom->fbo(1));
        blur->set_bool("horizontal", horizontal);
        glActiveTexture(GL_TEXTURE0);

        if (first_iteration)
            glBindTexture(GL_TEXTURE_2D, m_render_target->texture(index));
        else
            glBindTexture(GL_TEXTURE_2D,horizontal ? m_bloom->texture(1) : m_bloom->texture(0));

        render_quad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    uint32_t final_bloom_texture = horizontal ? m_bloom->texture(1) : m_bloom->texture(0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_render_target_secondary->framebuffer());
    glViewport(0, 0, width, height);

    auto* combine = resources->shader("bloom_final");

    combine->use();
    combine->set_int("scene", 0);
    combine->set_int("bloomBlur", 1);
    combine->set_float("exposure", 1.0f);
    combine->set_bool("bloom", true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_render_target->texture(0));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, final_bloom_texture);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    render_quad();

    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, m_render_target->framebuffer());
    std::swap(m_render_target, m_render_target_secondary);
    std::swap(m_final_texture, m_final_texture_secondary);
}

void GraphicsController::set_point_shadow_map(const resources::Shader *shader, int index) {

    auto *shadow_map = point_shadow_map(index);
    glActiveTexture(GL_TEXTURE10 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map->cubemap());

    shader->set_int("shadow_point_map[" + std::to_string(index) + "]", 10 + index);
}
void GraphicsController::terminate() {
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void GraphicsPlatformEventObserver::on_window_resize(int width, int height) {
    auto graphics = engine::core::Controller::get<GraphicsController>();
    m_graphics->perspective_params().Width = static_cast<float>(width);
    m_graphics->perspective_params().Height = static_cast<float>(height);
    m_graphics->orthographic_params().Right = static_cast<float>(width);
    m_graphics->orthographic_params().Top = static_cast<float>(height);

    graphics->m_render_target->resize(width, height);
    graphics->m_render_target_secondary->resize(width, height);

    CHECKED_GL_CALL(glViewport, 0, 0, width, height);
}

std::string_view GraphicsController::name() const {
    return "GraphicsController";
}

void GraphicsController::begin_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GraphicsController::end_gui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GraphicsController::draw_skybox(const resources::Shader *shader, const resources::Skybox *skybox) const {
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.view_matrix()));
    shader->use();
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection_matrix<>());
    CHECKED_GL_CALL(glDepthFunc, GL_LEQUAL);
    CHECKED_GL_CALL(glBindVertexArray, skybox->vao());
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, skybox->texture());
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 36);
    CHECKED_GL_CALL(glBindVertexArray, 0);
    CHECKED_GL_CALL(glDepthFunc, GL_LESS);// set depth function back to default
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);
}
int GraphicsController::generate_point_shadow_map(unsigned int size) {

    const unsigned int SHADOW_WIDTH = size, SHADOW_HEIGHT = size;

    unsigned int depth_map_fbo;
    unsigned int depth_cubemap;
    CHECKED_GL_CALL(glGenFramebuffers, 1, &depth_map_fbo);

    CHECKED_GL_CALL(glGenTextures, 1, &depth_cubemap);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, depth_cubemap);
    for (GLuint i = 0; i < 6; ++i)
        CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, depth_map_fbo);
    CHECKED_GL_CALL(glFramebufferTexture, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cubemap, 0);
    CHECKED_GL_CALL(glDrawBuffer, GL_NONE);
    CHECKED_GL_CALL(glReadBuffer, GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw util::Error("Point shadow framebuffer incomplete");

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_render_target->framebuffer());
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);

    m_point_shadow_maps.push_back(resources::PointShadowMap(depth_map_fbo, depth_cubemap, size));
    return static_cast<int>(m_point_shadow_maps.size()) - 1;
}
resources::PointShadowMap *GraphicsController::point_shadow_map(int i) {
    if (i < 0 || i >= m_point_shadow_maps.size()) {
        throw util::Error("m_point_shadow_maps index out of bounds");
    }
    return &m_point_shadow_maps[i];
}
void GraphicsController::draw_point_shadow_map(glm::vec3 position, const resources::PointShadowMap *map, const std::vector<std::string> &model_names, const std::vector<glm::vec3> &translations, const std::vector<float> &angle, const std::vector<glm::vec3> &axis, const std::vector<glm::vec3> &scale) const {
    const unsigned int SHADOW_WIDTH = map->size(), SHADOW_HEIGHT = map->size();

    float near_plane = 0.3f;
    float far_plane  = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), static_cast<float>(SHADOW_WIDTH) / static_cast<float>(SHADOW_HEIGHT), near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    glBindFramebuffer(GL_FRAMEBUFFER, map->fbo());
    glViewport(0, 0, static_cast<int>(SHADOW_WIDTH), static_cast<int>(SHADOW_HEIGHT));
    glClear(GL_DEPTH_BUFFER_BIT);
    auto resources = engine::core::Controller::get<resources::ResourcesController>();
    auto platform = engine::core::Controller::get<platform::PlatformController>();
    auto *depth = resources->shader("point_shadow_depth");

    depth->use();
    for (unsigned int i = 0; i < 6; ++i)
        depth->set_mat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

    depth->set_float("far_plane", far_plane);
    depth->set_vec3("lightPos", position);
    glCullFace(GL_FRONT);
    for (int i = 0; i < model_names.size(); i++) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, translations[i]);
        model = glm::rotate(model, glm::radians(angle[i]), axis[i]);
        model = glm::scale(model, scale[i]);

        depth->set_mat4("model", model);
        auto *mesh = resources->model(model_names[i]);
        mesh->draw(depth);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_render_target->framebuffer());
    glViewport(0, 0, platform->window()->width(), platform->window()->height());
    glCullFace(GL_BACK);

}
}// namespace engine::graphics
