#include <iostream>
#include <luna.hpp>
#include <imgui.h>

int main() {
    luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) {
        std::cout << '<' << prefix << "> " << message << std::endl;
    });

    luna::initialize();

    luna::Window window;
    luna::ForwardRenderer renderer;

    window.enableVSync(false);

    luna::Camera camera(&window);
    camera.setProjectionType(luna::ProjectionType::Perspective);
    camera.getTransform().position = glm::vec3(-1.0f);

    int selectedPrimitive = 0;
    const char* primitiveNames[] = { "Cube", "Quad", "Teapot" };
    const luna::Primitive primitives[] = { luna::Primitive::Cube, luna::Primitive::Quad, luna::Primitive::Teapot };
    unsigned cubeWidth = 5;

    while (!luna::isCloseRequested() && !window.isCloseRequested()) {
        luna::update();
        camera.updateAspect();
        renderer.beginFrame();

        if (luna::Input::isMouseButtonDown(luna::MouseButton::Right))
            luna::updateDebugCamera(camera);

#ifndef IMGUI_DISABLE
        if (ImGui::Begin("Benchmarking")) {

            ImGui::Text("Frametime: %ims", int(luna::getDeltaTime() * 1000.0));
            ImGui::Text("FPS: %i", int(1.0f / luna::getDeltaTime()));
            ImGui::Text("Objects: %i", cubeWidth * cubeWidth * cubeWidth);
            ImGui::Text("Triangles: %i", cubeWidth * cubeWidth * cubeWidth * luna::getPrimitive(primitives[selectedPrimitive])->vertexCount() / 3);


            ImGui::DragScalar("Cube Width", ImGuiDataType_U32, &cubeWidth, 0.2f);
            if (ImGui::BeginCombo("Primitive Type", primitiveNames[selectedPrimitive])) {
                for (int i = 0; i < sizeof(primitiveNames) / sizeof(*primitiveNames); i++) {
                    if (ImGui::Selectable(primitiveNames[i], i == selectedPrimitive))
                        selectedPrimitive = i;

                    if (i == selectedPrimitive)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            ImGui::End();
        }
#endif

        for (int x = 0; x < cubeWidth; x++) {
            for (int y = 0; y < cubeWidth; y++) {
                for (int z = 0; z < cubeWidth; z++) {
                    renderer.push(luna::getPrimitive(primitives[selectedPrimitive]), luna::Transform(glm::vec3(x, y, z) * 2.0f).matrix());
                }
            }
        }


        renderer.endFrame();
        renderer.render(camera);
        window.update();
    }

    luna::terminate();
}