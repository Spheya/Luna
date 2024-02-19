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

	while (!luna::isCloseRequested() && !window.isCloseRequested()) {
		luna::update();

		camera.updateAspect();
		renderer.beginFrame();

		renderer.push(
			luna::getPrimitive(luna::Primitive::Cube),
			luna::Transform(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(luna::getTime(), luna::getTime(), luna::getTime())).matrix()
		);

		renderer.endFrame();
		renderer.render(camera);

		window.update();
	}

	luna::terminate();
}