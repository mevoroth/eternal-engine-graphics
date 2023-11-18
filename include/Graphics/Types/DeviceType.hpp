#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType
		{
			DEVICE_TYPE_NULL,
			DEVICE_TYPE_PROXY,
			DEVICE_TYPE_D3D12,
			DEVICE_TYPE_VULKAN
#if ETERNAL_USE_PRIVATE
			#include "Graphics/Types/DeviceTypePrivate.hpp"
#else
			, DEVICE_TYPE_DEFAULT = DEVICE_TYPE_D3D12
#endif
		};
	}
}
