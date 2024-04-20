#pragma once

#include "Context.hpp"
#include "Camera.hpp"
#include "Input.hpp"

namespace luna {

	inline void updateDebugCamera(Camera& camera, float speed = 3.0f, float sensitivity = 0.5f) {
		glm::vec3 movement(
			(Input::isKeyDown(Key::D) || Input::isKeyDown(Key::Right)) - (Input::isKeyDown(Key::A) || Input::isKeyDown(Key::Left)),
			0.0f,
			(Input::isKeyDown(Key::S) || Input::isKeyDown(Key::Down)) - (Input::isKeyDown(Key::W) || Input::isKeyDown(Key::Up))
		);
		movement = camera.getTransform().rotationMatrix() * movement * speed;
		movement.y += (Input::isKeyDown(Key::Space) - (Input::isKeyDown(Key::LeftShift) || Input::isKeyDown(Key::RightShift))) * speed;

		camera.getTransform().position += movement * getDeltaTime();
		camera.getTransform().rotation += glm::vec3(Input::getMouseDelta().y * sensitivity * (Pi / 180.0), Input::getMouseDelta().x * sensitivity * (Pi / 180.0f), 0.0f);
		camera.getTransform().rotation.x = clamp(camera.getTransform().rotation.x, Tau * -0.25f, Tau * +0.25f);
	}

}