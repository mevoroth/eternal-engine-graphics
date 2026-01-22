#include "Graphics/GraphicsContextFactory.hpp"

#include "Null/NullGraphicsContext.hpp"
#include "Proxy/ProxyGraphicsContext.hpp"
#if ETERNAL_PLATFORM_WINDOWS
#include "Windows/d3d12/WindowsD3D12GraphicsContext.hpp"
#include "Windows/Vulkan/WindowsVulkanGraphicsContext.hpp"
#endif
#endif

namespace Eternal
{
	namespace Graphics
	{
		GraphicsContext* CreateGraphicsContext()
		{
			GraphicsContext* Context = new NullGraphicsContext();
			Context->InitializeGraphicsContext();
			return Context;
		}

#if ETERNAL_PLATFORM_WINDOWS
		GraphicsContext* CreateGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation)
		{
			GraphicsContext* Context = nullptr;

			switch (InWindowsGraphicsContextCreateInformation.Settings.Driver)
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				Context = new D3D12WindowsGraphicsContext(InWindowsGraphicsContextCreateInformation);
				break;
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				Context = new WindowsVulkanGraphicsContext(InWindowsGraphicsContextCreateInformation);
				break;
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}

			Context->InitializeGraphicsContext();

			return Context;
		}
#endif

#if ETERNAL_PLATFORM_ANDROID
		GraphicsContext* CreateGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InAndroidGraphicsContextCreateInformation)
		{
			GraphicsContext* Context = nullptr;

			//switch (InAndroidGraphicsContextCreateInformation.Settings.Driver)

			ETERNAL_BREAK();

			return Context;
		}
#endif

		GraphicsContext* CreateGraphicsContext(_In_ const ProxyGraphicsContextCreateInformation& InProxyGraphicsContextCreateInformation)
		{
			GraphicsContext* Context = new ProxyGraphicsContext(InProxyGraphicsContextCreateInformation);
			Context->InitializeGraphicsContext();
			return Context;
		}

		void DestroyGraphicsContext(_Inout_ GraphicsContext*& InOutContext)
		{
			delete InOutContext;
			InOutContext = nullptr;
		}

	}
}
