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

	camera.setBackgroundColor(luna::Color::Red);

	luna::Mesh quad{
		luna::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f)),
		luna::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)),
		luna::Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),

		luna::Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),
		luna::Vertex(glm::vec3(0.5f, 0.5f, 0.0f)),
		luna::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f)),
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
		void main() {\n\
			gl_Position = vec4(position, 1.0);\n\
			vertexColor = vec4(1.0, 1.0, 1.0, 1.0);\n\
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
	
	while (!context.isCloseRequested() && !window.isCloseRequested()) {
		renderer.beginFrame();

		renderer.push(&quad, glm::mat4(), &material);

		renderer.endFrame();
		renderer.render(camera);
		window.update();
	}
}