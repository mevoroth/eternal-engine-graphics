#include "NextGenGraphics/DeviceFactory.hpp"

#include "Macros/Macros.hpp"
#include "Log/Log.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Device.hpp"
#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Device* CreateDevice(_In_ const DeviceType& DeviceTypeObj, _In_ Window& WindowObj)
		{
			static bool InitDXGIFactory = false;

			switch (DeviceTypeObj)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				if (!InitDXGIFactory)
				{
					Eternal::Log::Log::Get()->Write(Eternal::Log::Log::Info, Eternal::Log::Log::Engine, "[Graphics::CreateDevice]Creating DXGI Factory");
					D3D12Device::Initialize();
					InitDXGIFactory = true;
				}
				Eternal::Log::Log::Get()->Write(Eternal::Log::Log::Info, Eternal::Log::Log::Engine, "[Graphics::CreateDevice]Creating Direct3D 12 Device");
				return new D3D12Device(0);
#endif

			case VULKAN:
				Eternal::Log::Log::Get()->Write(Eternal::Log::Log::Info, Eternal::Log::Log::Engine, "[Graphics::CreateDevice]Creating Vulkan Device");
				return new VulkanDevice(WindowObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
