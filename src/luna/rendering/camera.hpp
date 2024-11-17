#pragma once

#include "../platform/buffer.hpp"
#include "../platform/rendertarget.hpp"
#include "../core/math.hpp"

namespace luna {

	enum class ProjectionType {
		Perspective,
		Orthographic
	};

	struct CameraPropertyBlock {
		glm::mat4 projMat;
		glm::mat4 invProjMat;
		glm::mat4 viewMat;
		glm::mat4 invViewMat;
		glm::mat4 projViewMat;
		glm::uvec2 targetSize;
		glm::vec3 cameraPosition;
	};

	class Camera {
	public:
		explicit Camera(RenderTarget* target, const Transform& transform = Transform(glm::vec3(0.0f, 0.0f, 1.0f)));

		void setRenderTarget(RenderTarget* target);
		RenderTarget* renderTarget() const;

		void updateAspect();

		void setTransform(const Transform& transform);
		const Transform& transform() const;
		Transform& transform();

		ProjectionType projectionType() const;
		float near() const;
		float far() const;
		float aspectRatio() const;
		float fov() const;
		float orthographicSize() const;

		void setProjectionType(ProjectionType type);
		void setNear(float near);
		void setFar(float far);
		void setAspectRatio(float aspect);
		void setFov(float fov);
		void setOrthographicSize(float size);

		glm::mat4 projectionMatrix() const;
		glm::mat4 viewMatrix() const;
		CameraPropertyBlock propertyBlock() const;

	private:
		RenderTarget* m_target;

		Transform m_transform;
		ProjectionType m_projectionType = ProjectionType::Perspective;
		float m_near = 0.03f;
		float m_far = 1000.0f;
		float m_aspect = 1.0f;
		float m_fov = 70.0f;
		float m_orthographicSize = 5.0f;
	};

}