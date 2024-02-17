#include <iostream>

#include <luna.hpp>
#include <luna/Window.hpp>

#include <glad/glad.h>

int main() {
	luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) {
		std::cout << '<' << prefix << "> " << message << std::endl;
	});

	luna::Context context;
	luna::Window window(context, "Luna example");
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
		uniform sampler2D MainTexture;\n\
		\n\
		void main() {\n\
			gl_Position = vec4(position, 1.0);\n\
			vertexColor = texture(MainTexture, uv);\n\
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

	luna::Texture tex(luna::Color::Yellow);

	luna::Material material(&shader);
	material.setColor(luna::Color::Blue);
	material.setTexture(&tex);

	luna::Camera camera(&window);
	camera.setBackgroundColor(luna::Color::Magenta);

	while (!context.isCloseRequested() && !window.isCloseRequested()) {
		auto err = glGetError();
		if (err)
			std::cout << "GL Error: " << std::to_string(err) << std::endl;

		renderer.beginFrame();

		renderer.push(&quad, glm::mat4(), &material);

		renderer.endFrame();
		renderer.render(camera);
		window.update();
	}
}