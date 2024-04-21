#include <iostream>
#include <luna.hpp>
#include <imgui.h>

int main() {
    luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) {
        std::cout << '<' << prefix << "> " << message << std::endl;
    });

    luna::initialize();

    luna::Window window;
    luna::ForwardRenderer renderer;

    luna::Camera camera(&window);
    camera.setProjectionType(luna::ProjectionType::Perspective);

    while (!luna::isCloseRequested() && !window.isCloseRequested()) {
        luna::update();
        camera.updateAspect();
        renderer.beginFrame();

        if (luna::Input::isMouseButtonDown(luna::MouseButton::Right))
            luna::updateDebugCamera(camera);

#ifndef IMGUI_DISABLE
        ImGui::ShowDemoWindow();
#endif
        glm::mat4 teapotMatrix = luna::Transform(glm::vec3(1.5f, 0.0f, -5.0f), glm::vec3(luna::getTime(), luna::getTime(), luna::getTime())).matrix();
        glm::mat4 cubeMatrix = luna::Transform(glm::vec3(-1.5f, 0.0f, -5.0f), glm::vec3(luna::getTime(), luna::getTime(), luna::getTime())).matrix();

        renderer.push(luna::getPrimitive(luna::Primitive::Teapot), teapotMatrix);
        renderer.push(luna::getPrimitive(luna::Primitive::Cube), cubeMatrix);

        renderer.endFrame();
        renderer.render(camera);
        window.update();
    }

    luna::terminate();
}