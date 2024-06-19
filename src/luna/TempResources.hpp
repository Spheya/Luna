#pragma once

#include "RenderTexture.hpp"

namespace luna {

	template<typename T, void(*Release)(T*)>
	class TempResource {
	public:
		using ResourceType = T;

		explicit TempResource(T* value) : m_value(value) {}
		TempResource(TempResource&) = delete;
		TempResource& operator=(TempResource&) = delete;
		TempResource(TempResource&& other) noexcept : m_value(other.m_value) { other.m_value = nullptr; }
		TempResource& operator=(TempResource&& other) noexcept(noexcept(Release())) { if (m_value) Release(m_value); m_value = other.m_value; other.m_value = nullptr; };
		~TempResource() { if(m_value) Release(m_value); }

		T& operator*() { return *m_value; }
		const T& operator*() const { return *m_value; }
		T* operator->() { return m_value; }
		const T* operator->() const { return m_value; }

		T* get() { return m_value; };
		const T* get() const { return m_value; }

	private:
		T* m_value;
	};
	
	void updateTempResources();
	void releaseAllTempResources() noexcept;

	void releaseTempRenderTexture(RenderTexture* texture) noexcept;
	using TempRenderTexture = TempResource <RenderTexture, releaseTempRenderTexture>;
	TempRenderTexture getTempRenderTexture(glm::ivec2 size, luna::TextureFormat format = luna::TextureFormat::Rgba, unsigned int retention = 1);
	TempRenderTexture getTempRenderTexture(int width, int height, luna::TextureFormat format = luna::TextureFormat::Rgba, unsigned int retention = 1);
}