#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum DeviceType
		{
			D3D12,
			VULKAN
		};

		class Device
		{
		public:
			virtual uint32_t GetDeviceMask() const = 0;
			virtual DeviceType GetDeviceType() const = 0;
		};
	}
}

#endif
