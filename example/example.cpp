#include <iostream>

#include <luna.hpp>
#include <luna/Window.hpp>

int main() {
	luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) {
		std::cout << '<' << prefix << "> " << message << std::endl;
	});

	luna::Context context;
	luna::Window window(context, "Luna example");
	luna::Camera camera(&window);
	luna::Renderer renderer;

	luna::Mesh quad{
		luna::Vertex(glm::vec3(-0.5f, +0.5f, 0.0f), glm::vec2(0.0f, 0.0f)),
		luna::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 1.0f)),
		luna::Vertex(glm::vec3(+0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 1.0f)),

		luna::Vertex(glm::vec3(+0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 1.0f)),
		luna::Vertex(glm::vec3(+0.5f, +0.5f, 0.0f), glm::vec2(1.0f, 0.0f)),
		luna::Vertex(glm::vec3(-0.5f, +0.5f, 0.0f), glm::vec2(0.0f, 0.0f)),
	};

	luna::Shader shader(
		"\
		#version 330 core\n\
		\n\
		in vec3 position;\n\
		in vec2 uv;\n\
		in vec3 normal;\n\
		\n\
		out vec4 vertexColor;\n\
		\n\
		uniform vec4 MainColor;\n\
		\n\
		void main() {\n\
			gl_Position = vec4(position, 1.0);\n\
			vertexColor = MainColor;\n\
		}\n",
		
		"\
		#version 330 core\n\
		\n\
		in vec4 vertexColor;\n\
		\n\
		out vec4 FragColor;\n\
		\n\
		void main() {\n\
			FragColor = vertexColor;\n\
		}\n"
	);

	luna::Material material(&shader);
	material.setColor(luna::Color::Blue);

	while (!context.isCloseRequested() && !window.isCloseRequested()) {
		renderer.beginFrame();

		renderer.push(&quad, glm::mat4(), &material);

		renderer.endFrame();
		renderer.render(camera);
		window.update();
	}
}