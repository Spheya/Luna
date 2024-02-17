#include <iostream>

#include <luna.hpp>
#include <luna/Window.hpp>

#include <glad/glad.h>

#include <thread>

int main() {
	luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) {
		std::cout << '<' << prefix << "> " << message << std::endl;
		});

	luna::Context context;
	luna::Window window(context, "Luna example");
	luna::Renderer renderer;

	luna::Shader shader(
		"\
		#version 330 core\n\
		\n\
		layout(location = 0) in vec3 Position;\n\
		layout(location = 1) in vec2 UV;\n\
		layout(location = 2) in vec3 Normal;\n\
		\n\
		out vec4 vertexColor;\n\
		\n\
		uniform vec4 MainColor;\n\
		uniform sampler2D MainTexture;\n\
		\n\
		uniform mat4 ProjectionMatrix;\n\
		uniform mat4 ViewMatrix;\n\
		uniform mat4 ModelMatrix;\n\
		\n\
		void main() {\n\
			gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);\n\
			vertexColor = vec4(Normal * 0.5 + 0.5, 0.0);\n\
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
	camera.setProjectionType(luna::ProjectionType::Perspective);

	luna::Transform quadTransform(glm::vec3(0.0f, 0.0f, -3.0f));

	luna::Window window2(context, "Example 2");

	while (!context.isCloseRequested() && !window.isCloseRequested()) {
		context.update();
		camera.updateAspect();
		renderer.beginFrame();

		quadTransform.rotation = glm::vec3(context.getTime(), context.getTime(), context.getTime());
		renderer.push(context.getPrimitive(luna::Primitive::Cube), quadTransform.matrix(), &material);

		renderer.endFrame();
		camera.setTarget(&window);
		renderer.render(camera);
		camera.setTarget(&window2);
		renderer.render(camera);

		window.update();
		window2.update();
	}
}