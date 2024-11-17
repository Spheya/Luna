#pragma once

#include <string>

namespace luna {

	using RenderType = size_t;

	static constexpr RenderType NullRenderType = 0;

	RenderType registerRenderType(const std::string& renderType);
	RenderType getRenderTypeId(std::string_view renderType);
	const std::string& getRenderTypeName(RenderType renderType);
	size_t getRenderTypesCount();

}