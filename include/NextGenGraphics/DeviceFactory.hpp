#ifndef _DEVICE_FACTORY_HPP_
#define _DEVICE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Window;
		class GraphicsContext;
		enum class DeviceType;

		Device* CreateDevice(_In_ const DeviceType& DeviceTypeObj, _In_ Window& WindowObj);
		Device* CreateDevice(_Inout_ GraphicsContext& Context, _In_ const DeviceType& DeviceTypeObj);
	}
}

#endif
