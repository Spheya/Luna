#include <iostream>
#include <luna.hpp>
#include <luna/Window.hpp>

int main() {
	luna::Window window("luna example");
	if (!window.isValid()) {
		std::cout << "error setting up window!" << std::endl;
		return -1;
	}

	luna::Camera camera(&window);
	luna::Renderer renderer;

	luna::Mesh quad {
		luna::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f)),
		luna::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)),
		luna::Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),

		luna::Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),
		luna::Vertex(glm::vec3(0.5f, 0.5f, 0.0f)),
		luna::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f)),
	};

	while (!window.isCloseRequested()) {
		window.clear(luna::Color::blue);
		renderer.beginFrame();

		renderer.push(&quad);

		renderer.render(camera);
		window.update();
	}
}