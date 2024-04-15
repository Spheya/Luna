#include <iostream>
#include <luna.hpp>

int main() {
    luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) {
        std::cout << '<' << prefix << "> " << message << std::endl;
    });

    luna::initialize();

    luna::Window window;
    luna::Renderer renderer;

    luna::Camera camera(&window);
    camera.setProjectionType(luna::ProjectionType::Perspective);
    camera.setBackgroundColor(luna::Color::White);

    luna::Material material(luna::getDefaultShader());
    luna::Texture texture = luna::Texture::loadFromFile("assets/Blobby.png");

    material.setMainTexture(&texture);

    while (!luna::isCloseRequested() && !window.isCloseRequested()) {
        luna::update();
        camera.updateAspect();
        renderer.beginFrame();

        glm::vec3 textureScale(texture.getWidth() / 16.0f, texture.getHeight() / 16.0f, 1.0f);

        renderer.push(
            luna::getPrimitive(luna::Primitive::Quad),
            luna::Transform(glm::vec3(0.0f), glm::vec3(0.0f), textureScale).matrix(),
            &material
        );

        renderer.endFrame();
        renderer.render(camera);
        window.update();
    }

    luna::terminate();
}