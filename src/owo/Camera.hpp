#pragma once

#include "Math.hpp"
#include "RenderTarget.hpp"

namespace owo {

	class Camera {
	public:
		enum class ProjectionType : std::uint8_t { Perspective, Orthographic };

		explicit Camera(RenderTarget* target, Transform transform = Transform(glm::vec3(0.0f, 0.0f, 1.0f)));

		void setTarget(RenderTarget* target);
		RenderTarget* getTarget() const;
		void setTransform(const Transform& transform);
		const Transform& getTransform() const;
		Transform& getTransform();
		ProjectionType getProjectionType() const;
		void setProjectionType(ProjectionType type);
		float getNear() const;
		void setNear(float near);
		float getFar() const;
		void setFar(float far);
		float getAspect() const;
		void setAspect(float aspect);
		float getFov() const;
		void setFov(float fov);
		float getOrthographicSize() const;
		void setOrthographicSize(float size);

		glm::mat4 projection() const;

	private:
		RenderTarget* m_target;
		Transform m_transform;
		ProjectionType m_projectionType = ProjectionType::Orthographic;

		float m_near = 0.03f;
		float m_far = 1000.0f;
		float m_aspect = 1.0f;
		float m_fov = 70.0f;
		float m_orthographicSize = 5.0f;
	};

}