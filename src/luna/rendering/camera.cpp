#include "camera.hpp"

namespace luna {

	Camera::Camera(RenderTarget* target, const Transform& transform) :
		m_target(target),
		m_transform(transform)
	{}

	void Camera::setRenderTarget(RenderTarget* target) {
		m_target = target;
	}

	RenderTarget* Camera::renderTarget() const {
		return m_target;
	}

	void Camera::updateAspect() {
		if(m_target)
			m_aspect = float(m_target->framebufferWidth()) / float(m_target->framebufferHeight());
	}

	void Camera::setTransform(const Transform& transform) {
		m_transform = transform;
	}

	const Transform& Camera::transform() const {
		return m_transform;
	}

	Transform& Camera::transform() {
		return m_transform;
	}

	ProjectionType Camera::projectionType() const {
		return m_projectionType;
	}

	float Camera::aspectRatio() const {
		return m_aspect;
	}

	float Camera::fov() const {
		return m_fov;
	}

	float Camera::orthographicSize() const {
		return m_orthographicSize;
	}

	void Camera::setProjectionType(ProjectionType type) {
		m_projectionType = type;
	}

	void Camera::setNear(float near) {
		m_near = near;
	}

	void Camera::setFar(float far) {
		m_far = far;
	}

	void Camera::setAspectRatio(float aspect) {
		m_aspect = aspect;
	}

	void Camera::setFov(float fov) {
		m_fov = fov;
	}

	void Camera::setOrthographicSize(float size) {
		m_orthographicSize = size;
	}

	glm::mat4 Camera::projectionMatrix() const {
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

	glm::mat4 Camera::viewMatrix() const {
		return m_transform.inverseMatrix();
	}

	CameraPropertyBlock Camera::propertyBlock() const {
		auto projMat = projectionMatrix();
		auto viewMat = viewMatrix();
		return { projMat, inverse(projMat), viewMat, m_transform.matrix(), projMat * viewMat, m_target ? glm::uvec2(1,1) : m_target->framebufferSize(), m_transform.position };
	}

}