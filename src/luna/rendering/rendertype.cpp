#include "rendertype.hpp"

#include <cassert>
#include <vector>

namespace luna {

	static std::vector<std::string> renderTypes;

	RenderType registerRenderType(const std::string& renderType) {
		assert(std::find(renderTypes.begin(), renderTypes.end(), renderType) == renderTypes.end());
		renderTypes.push_back(renderType);
		return renderTypes.size();
	}

	RenderType getRenderTypeId(std::string_view renderType) {
		for (size_t i = 0; i < renderTypes.size(); ++i)
			if (renderTypes[i] == renderType)
				return i + 1;
		return 0;
	}

	const std::string& getRenderTypeName(RenderType renderType) {
		return renderTypes[renderType - 1];
	}

	size_t getRenderTypesCount() {
		return renderTypes.size() + 1; // 1 extra because of NullRenderType
	}

}
