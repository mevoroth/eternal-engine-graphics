#pragma once

#include <string>

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
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
			#include "Graphics/Types/DeviceTypeEnumPrivate.hpp"
#else
			, DEVICE_TYPE_COUNT
			#if ETERNAL_PLATFORM_WINDOWS
			, DEVICE_TYPE_DEFAULT = DEVICE_TYPE_D3D12
			#endif
			#if ETERNAL_PLATFORM_ANDROID
			, DEVICE_TYPE_DEFAULT = DEVICE_TYPE_VULKAN
			#endif
#endif
			, DEVICE_TYPE_BASE_COUNT = DEVICE_TYPE_VULKAN + 1
			, DEVICE_TYPE_EXTENDED_COUNT = DEVICE_TYPE_COUNT - DEVICE_TYPE_BASE_COUNT
		};

		const char* ConvertDeviceTypeToString(_In_ const DeviceType& InDeviceType);
		DeviceType ConvertStringToDeviceType(_In_ const std::string& InDeviceTypeString);
	}
}
