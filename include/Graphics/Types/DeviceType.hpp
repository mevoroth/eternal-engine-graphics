#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType
		{
			DEVICE_TYPE_D3D12,
			DEVICE_TYPE_VULKAN
		#if ETERNAL_USE_PRIVATE
			#include "GraphicsPrivate/Types/DeviceTypePrivate.hpp"
		#else
			, DEVICE_TYPE_DEFAULT = DEVICE_TYPE_D3D12
		#endif
		};
	}
}
