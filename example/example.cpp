#include <iostream>
#include <owo.hpp>
#include <owo/Window.hpp>

int main() {
	owo::Window window("owo example");
	if (!window.isValid()) {
		std::cout << "error setting up window!" << std::endl;
		return -1;
	}

	owo::Camera camera(&window);
	owo::Renderer renderer;

	owo::Mesh quad {
		owo::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f)),
		owo::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)),
		owo::Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),

		owo::Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),
		owo::Vertex(glm::vec3(0.5f, 0.5f, 0.0f)),
		owo::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f)),
	};

	while (!window.isCloseRequested()) {
		window.clear();
		renderer.beginFrame();

		renderer.push(&quad);

		renderer.render(camera);
		window.update();
	}
}