#include "Graphics/Types/DeviceType.hpp"
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
#include "Graphics/Types/DeviceTypePrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		static constexpr char* DEVICE_TYPE_TO_STRINGS[] =
		{
			"Null",
			"Proxy",
			"D3D12",
			"Vulkan"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(DEVICE_TYPE_TO_STRINGS) == static_cast<uint32_t>(DeviceType::DEVICE_TYPE_BASE_COUNT), "Mismatch on DeviceType strings");

		const char* ConvertDeviceTypeToString(_In_ const DeviceType& InDeviceType)
		{
			uint32_t InDeviceTypeInt = static_cast<uint32_t>(InDeviceType);
			if (InDeviceTypeInt < static_cast<uint32_t>(DeviceType::DEVICE_TYPE_BASE_COUNT))
				return DEVICE_TYPE_TO_STRINGS[InDeviceTypeInt];

#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
			return ConvertDeviceTypeToStringPrivate(InDeviceType);
#endif
			ETERNAL_BREAK();
			return "";
		}

		DeviceType ConvertStringToDeviceType(_In_ const std::string& InDeviceTypeString)
		{
			for (uint32_t DeviceTypeIndex = 0; DeviceTypeIndex < static_cast<uint32_t>(DeviceType::DEVICE_TYPE_COUNT); ++DeviceTypeIndex)
			{
				if (InDeviceTypeString == ConvertDeviceTypeToString(static_cast<DeviceType>(DeviceTypeIndex)))
					return static_cast<DeviceType>(DeviceTypeIndex);
			}
			return DeviceType::DEVICE_TYPE_NULL;
		}
	}
}
