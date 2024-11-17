#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <format>

namespace luna {

	constexpr float Pi          = 3.14159265359f;
	constexpr float Tau         = 6.28318530718f;
	constexpr float E           = 2.71828182845f;
	constexpr float GoldenRatio = 1.61803398874f;
	constexpr float Sqrt2       = 1.41421356237f;
	constexpr float Sqrt3       = 1.73205080757f;
	constexpr float Sqrt5       = 2.23606797749f;
	constexpr float RadToDeg    = 57.2957795131f;
	constexpr float DegToRad    = 0.01745329251f;

	struct Transform {
		explicit Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) :
			position(position),
			rotation(rotation),
			scale(scale) {
		}

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

template<typename T, glm::qualifier Q>
struct std::formatter<glm::vec<1, T, Q>> : std::formatter<std::string> {
	auto format(glm::vec<1, T, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{}]", obj.x), ctx);
	}
};

template<typename T, glm::qualifier Q>
struct std::formatter<glm::vec<2, T, Q>> : std::formatter<std::string> {
	auto format(glm::vec<2, T, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{}, {}]", obj.x, obj.y), ctx);
	}
};

template<typename T, glm::qualifier Q>
struct std::formatter<glm::vec<3, T, Q>> : std::formatter<std::string> {
	auto format(glm::vec<3, T, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{}, {}, {}]", obj.x, obj.y, obj.z), ctx);
	}
};

template<typename T, glm::qualifier Q>
struct std::formatter<glm::vec<4, T, Q>> : std::formatter<std::string> {
	auto format(glm::vec<4, T, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{}, {}, {}, {}]", obj.x, obj.y, obj.z, obj.w), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<1, float, Q>> : std::formatter<std::string> {
	auto format(glm::vec<1, float, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}]", obj.x), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<2, float, Q>> : std::formatter<std::string> {
	auto format(glm::vec<2, float, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}, {:.2f}]", obj.x, obj.y), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<3, float, Q>> : std::formatter<std::string> {
	auto format(glm::vec<3, float, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}, {:.2f}, {:.2f}]", obj.x, obj.y, obj.z), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<4, float, Q>> : std::formatter<std::string> {
	auto format(glm::vec<4, float, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}, {:.2f}, {:.2f}, {:.2f}]", obj.x, obj.y, obj.z, obj.w), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<1, double, Q>> : std::formatter<std::string> {
	auto format(glm::vec<1, double, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}]", obj.x), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<2, double, Q>> : std::formatter<std::string> {
	auto format(glm::vec<2, double, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}, {:.2f}]", obj.x, obj.y), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<3, double, Q>> : std::formatter<std::string> {
	auto format(glm::vec<3, double, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}, {:.2f}, {:.2f}]", obj.x, obj.y, obj.z), ctx);
	}
};

template<glm::qualifier Q>
struct std::formatter<glm::vec<4, double, Q>> : std::formatter<std::string> {
	auto format(glm::vec<4, double, Q> obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("[{:.2f}, {:.2f}, {:.2f}, {:.2f}]", obj.x, obj.y, obj.z, obj.w), ctx);
	}
};

template<>
struct std::formatter<luna::Transform> : std::formatter<std::string> {
	auto format(luna::Transform obj, std::format_context& ctx) const {
		return formatter<string>::format(std::format("{{ position: {}, rotation: {}, scale: {} }}", obj.position, obj.rotation, obj.scale), ctx);
	}
};