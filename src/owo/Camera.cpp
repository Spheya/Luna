#include "Camera.hpp"

namespace owo {
	Camera::Camera(RenderTarget* target, Transform transform) :
		m_target(target),
		m_transform(transform)
	{}

	void Camera::setTarget(RenderTarget* target) {
		m_target = target;
	}

	RenderTarget* Camera::getTarget() const {
		return m_target;
	}

	void Camera::setTransform(const Transform& transform) {
		m_transform = transform;
	}

	const Transform& Camera::getTransform() const {
		return m_transform;
	}

	Transform& Camera::getTransform() {
		return m_transform;
	}

	Camera::ProjectionType Camera::getProjectionType() const {
		return m_projectionType;
	}

	void Camera::setProjectionType(ProjectionType type) {
		m_projectionType = type;
	}

	float Camera::getNear() const {
		return m_near;
	}

	void Camera::setNear(float near) {
		m_near = near;
	}

	float Camera::getFar() const {
		return m_far;
	}

	void Camera::setFar(float far) {
		m_far = far;
	}

	float Camera::getAspect() const {
		return m_aspect;
	}

	void Camera::setAspect(float aspect) {
		m_aspect = aspect;
	}

	float Camera::getFov() const {
		return m_fov;
	}

	void Camera::setFov(float fov) {
		m_fov = fov;
	}

	float Camera::getOrthographicSize() const {
		return m_orthographicSize;
	}

	void Camera::setOrthographicSize(float size) {
		m_orthographicSize = size;
	}

	glm::mat4 Camera::projection() const {
		if (m_projectionType == ProjectionType::Orthographic) {
			float left = -m_orthographicSize * m_aspect * 0.5f;
			float right = m_orthographicSize * m_aspect * 0.5f;
			float bottom = -m_orthographicSize * 0.5f;
			float top = m_orthographicSize * 0.5f;
			return glm::ortho(left, right, bottom, top, m_near, m_far);
		} else {
			return glm::perspective(m_fov, m_aspect, m_near, m_far);
		}
	}
}
