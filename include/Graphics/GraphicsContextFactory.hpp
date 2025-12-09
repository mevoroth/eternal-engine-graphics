#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType;
		class GraphicsContext;
		struct WindowsGraphicsContextCreateInformation;
		struct AndroidGraphicsContextCreateInformation;
		struct ProxyGraphicsContextCreateInformation;

		GraphicsContext* CreateGraphicsContext();
		GraphicsContext* CreateGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation);
		GraphicsContext* CreateGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InAndroidGraphicsContextCreateInformation);
		GraphicsContext* CreateGraphicsContext(_In_ const ProxyGraphicsContextCreateInformation& InProxyGraphicsContextCreateInformation);
		void DestroyGraphicsContext(_Inout_ GraphicsContext*& Context);
	}
}

#if ETERNAL_USE_PRIVATE
#include "Graphics/GraphicsContextFactoryPrivate.hpp"
#endif
