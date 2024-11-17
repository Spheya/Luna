#include <luna.hpp>

int main() {
	luna::setMessageCallback([](std::string_view message, luna::MessageSeverity severity) {
		switch (severity) {
		case luna::MessageSeverity::Trace:   std::cout << "[Trace]"; break;
		case luna::MessageSeverity::Info:    std::cout << "\033[32m[Info]"; break;
		case luna::MessageSeverity::Warning: std::cout << "\033[33m[Warning]"; break;
		case luna::MessageSeverity::Error:   std::cout << "\033[31m[Error]"; break;
		}
		std::cout << " Luna: \033[0m" << message << std::endl;
	});

	luna::Window window;

	luna::initialize("Luna Example");
	{
		luna::Camera camera(&window);

		luna::ForwardRenderer renderer;

		luna::Texture texture = luna::Texture(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		texture.setFilter(luna::TextureFilter::Nearest);
		texture.generateMipmap();

		luna::Shader shader(
			16,
			{ "_MainTexture" },
			{
				luna::ShaderSource::fromFile("assets/shader.vsh", luna::ShaderStage::Vertex),
				luna::ShaderSource::fromFile("assets/shader.fsh", luna::ShaderStage::Fragment)
			}
		);

		luna::Material mat(&shader);
		glm::vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
		mat.setMaterialBlockData(&col);
		mat.setSampler("_MainTexture", &texture);

		luna::Transform transform(glm::vec3(0.0f, 0.0f, -5.0f));

		while (!window.isCloseRequested()) {
			luna::update();
			camera.updateAspect();

			transform.rotation += luna::deltatime();
			renderer.submit(&luna::getPrimitive(luna::Primitive::Cube), &mat, transform.matrix());

			renderer.render(camera);
			renderer.clear();
			window.update();
		}
	}
	luna::terminate();
}