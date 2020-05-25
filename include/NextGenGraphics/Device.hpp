#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType;

		class Device
		{
		public:
			virtual uint32_t GetDeviceMask() const = 0;
			virtual DeviceType GetDeviceType() const = 0;
		};
	}
}

#endif
