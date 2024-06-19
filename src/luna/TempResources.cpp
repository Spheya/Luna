#include "TempResources.hpp"

#include "Logger.hpp"

#include <memory>
#include <vector>

namespace luna {

	namespace {
		template<typename T>
		struct StoredTmpResource {
			std::unique_ptr<T> resource;
			bool available;
			unsigned int lifetime;
		};

		std::vector<StoredTmpResource<RenderTexture>> tempRenderTextures;
	}

	void updateTempResources() {
		for (auto it = tempRenderTextures.begin(); it != tempRenderTextures.end();) {
#ifdef _DEBUG
			if (!it->available) {
				log("A TempRenderTexture has not been released before calling luna::update()", MessageSeverity::Error);
				continue;
			}
#endif

			--(it->lifetime);
			if (it->lifetime <= 0) {
				*it = std::move(tempRenderTextures.back());
				tempRenderTextures.pop_back();
			} else {
				++it;
			}
		}
	}

	void releaseAllTempResources() noexcept {
		tempRenderTextures.clear();
	}

	void releaseTempRenderTexture(RenderTexture* texture) noexcept {
		for (auto& tmpRenderTexture : tempRenderTextures) {
			if (tmpRenderTexture.resource.get() == texture) {
#ifdef _DEBUG
				if (tmpRenderTexture.available)
					log("A TempRenderTexture got released twice! Remember that they get released automatically in their destructor", MessageSeverity::Warning);
#endif

				tmpRenderTexture.available = true;
				return;
			}
		}

#ifdef _DEBUG
		log("Attempt to release a TempRenderTexture that does not exist", MessageSeverity::Warning);
#endif
	}

	TempRenderTexture getTempRenderTexture(glm::ivec2 size, luna::TextureFormat format, unsigned int retention) {
		for (auto& tmpRenderTexture : tempRenderTextures) {
			auto* resource = tmpRenderTexture.resource.get();
			if (tmpRenderTexture.available && resource->getSize() == size && resource->getFormat() == format) {
				tmpRenderTexture.available = false;
				resource->makeActiveTarget();
				RenderTarget::clear(Color::Clear);
				return TempRenderTexture(resource);
			}
		}

		tempRenderTextures.emplace_back(std::make_unique<RenderTexture>(size, format), false, retention);
		return TempRenderTexture(tempRenderTextures.back().resource.get());
	}

	TempRenderTexture getTempRenderTexture(int width, int height, luna::TextureFormat format, unsigned int retention) {
		return getTempRenderTexture(glm::ivec2(width, height), format, retention);
	}
}