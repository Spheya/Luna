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

		luna::Texture texture = luna::Texture::fromFile("assets/jaccobox.png");
		texture.generateMipmap();
		luna::Texture texture2(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

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

		luna::Material mat2(&shader);
		glm::vec4 col2(1.0f, 0.0f, 1.0f, 1.0f);
		mat2.setMaterialBlockData(&col2);
		mat2.setSampler("_MainTexture", &texture2);

		luna::Material mat3(&shader);
		mat3.setMaterialBlockData(&col);
		mat3.setSampler("_MainTexture", renderer.m_shadowMap.depthBuffer());

		luna::Transform transform(glm::vec3(0.0f, 0.0f, -2.0f));
		luna::Transform transform1(glm::vec3(+0.8f, 0.0f, -5.0f));
		luna::Transform transform2(glm::vec3(-0.8f, 0.0f, -5.0f));
		luna::Transform transform3(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.0f), glm::vec3(10.0f, 1.0f, 10.0f));

		while (!window.isCloseRequested()) {
			luna::update();
			camera.updateAspect();

			transform1.rotation += luna::deltatime();
			transform2.rotation += luna::deltatime();

			renderer.submit(&luna::getPrimitive(luna::Primitive::Teapot), &mat2, transform1.matrix());
			//renderer.submit(&luna::getPrimitive(luna::Primitive::Cube), &mat, transform2.matrix());
			//renderer.submit(&luna::getPrimitive(luna::Primitive::Cube), &mat, transform3.matrix());
			//renderer.submit(&luna::getPrimitive(luna::Primitive::Quad), &mat3, transform.matrix());
			//renderer.submit(luna::DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.7f, glm::normalize(glm::vec3(1.0f, -2.0f, -1.0f))));

			renderer.render(camera);

			renderer.clear();
			window.update();
		}
	}
	luna::terminate();
}