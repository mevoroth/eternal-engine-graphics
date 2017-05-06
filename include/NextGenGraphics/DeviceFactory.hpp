#ifndef _DEVICE_FACTORY_HPP_
#define _DEVICE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Window;
		enum DeviceType;

		Device* CreateDevice(_In_ const DeviceType& DeviceTypeObj, _In_ Window& WindowObj);
	}
}

#endif
