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
    camera.setProjectionType(luna::ProjectionType::Orthographic);
    camera.setBackgroundColor(luna::Color::White);

    luna::Material material(luna::getDefaultShader());
    luna::Material solidMaterial(luna::getDefaultShader());
    luna::Texture transparentTexture = luna::Texture::loadFromFile("assets/BlobbyTransparent.png");
    luna::Texture texture = luna::Texture::loadFromFile("assets/Blobby.png");

    material.setMainTexture(&transparentTexture);
    solidMaterial.setMainTexture(&texture);

    while (!luna::isCloseRequested() && !window.isCloseRequested()) {
        luna::update();
        camera.updateAspect();
        renderer.beginFrame();

        glm::vec3 textureScale(texture.getWidth() / 16.0f, texture.getHeight() / 16.0f, 1.0f);

        renderer.push(
            luna::getPrimitive(luna::Primitive::Quad),
            luna::Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), textureScale).matrix(),
            &material
        );
        renderer.push(
            luna::getPrimitive(luna::Primitive::Quad),
            luna::Transform(glm::vec3(0.5f, 0.25f, 0.1f), glm::vec3(0.0f), textureScale).matrix(),
            &material
        );
        renderer.push(
            luna::getPrimitive(luna::Primitive::Quad),
            luna::Transform(glm::vec3(-0.25f, -0.25f, 0.05f), glm::vec3(0.0f), textureScale).matrix(),
            &solidMaterial
        );
        renderer.push(
            luna::getPrimitive(luna::Primitive::Quad),
            luna::Transform(glm::vec3(-0.25f, 0.25f, -0.1f), glm::vec3(0.0f), textureScale).matrix(),
            &solidMaterial
        );

        renderer.endFrame();
        renderer.render(camera);
        window.update();
    }

    luna::terminate();
}