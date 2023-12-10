#include "Proxy/ProxyDevice.hpp"
#include "Proxy/ProxyGraphicsContext.hpp"
#include "Graphics/DeviceFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ProxyDevice::ProxyDevice(_Inout_ GraphicsContext& InOutContext)
			: _Context(InOutContext)
		{
			// Graphics context is not fully initialized yet
			PreInitializeDevice(InOutContext, static_cast<const ProxyGraphicsContextCreateInformation&>(InOutContext.GetGraphicsContextCreateInformation()).ProxyDeviceType);
		}

		ProxyDevice::~ProxyDevice()
		{
			PreDestroyDevice(static_cast<ProxyGraphicsContext&>(_Context).GetProxyDeviceType());
		}
	}
}
