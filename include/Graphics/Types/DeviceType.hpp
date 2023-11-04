#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType
		{
			D3D12,
			VULKAN
			#if ETERNAL_USE_GRAPHICS_PRIVATE
			#include "GraphicsPrivate/Types/DeviceType.hpp"
			#endif
		};
	}
}
