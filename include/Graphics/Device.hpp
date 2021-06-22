#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType;
		class CommandList;

		class Device
		{
		public:
			virtual uint32_t GetDeviceMask() const = 0;
			virtual DeviceType GetDeviceType() const = 0;
		};
	}
}
