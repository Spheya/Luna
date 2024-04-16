#pragma once

#include <cstdint>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace luna {
	using namespace glm;

	constexpr float Pi          = 3.14159265359f;
	constexpr float Tau         = 6.28318530718f;
	constexpr float E           = 2.71828182845f;
	constexpr float GoldenRatio = 1.61803398874f;
	constexpr float Sqrt2       = 1.41421356237f;
	constexpr float Sqrt3       = 1.73205080757f;
	constexpr float Sqrt5       = 2.23606797749f;

	struct Transform {
		explicit Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) :
			position(position),
			rotation(rotation),
			scale(scale)
		{}

		glm::vec3 position;
		glm::vec3 rotation; // TODO: quaternions
		glm::vec3 scale;

		glm::mat3 rotationMatrix() const {
			return glm::toMat3(glm::quat(rotation));
		}

		glm::mat4 matrix() const {
			return glm::translate(glm::mat4(1.0f), position)
				 * glm::toMat4(glm::quat(rotation))
				 * glm::scale(glm::mat4(1.0f), scale);
		}

		glm::mat4 inverseMatrix() const {
			return glm::inverse(matrix());
		}
	};

}