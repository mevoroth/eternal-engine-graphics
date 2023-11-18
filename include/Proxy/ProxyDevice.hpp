#pragma once

#include "Graphics/Device.hpp"
#include "Graphics/Types/DeviceType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class ProxyDevice final : public Device
		{
		public:

			virtual uint32_t GetDeviceMask() const override final { return 0x1; };
			virtual DeviceType GetDeviceType() const override final { return DeviceType::DEVICE_TYPE_PROXY; };
		};
	}
}
