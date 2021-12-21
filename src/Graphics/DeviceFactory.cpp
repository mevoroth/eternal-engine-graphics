#include "Graphics/DeviceFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Device.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Device* CreateDevice(_In_ const DeviceType& InDeviceType, _In_ Window& InWindow)
		{
			switch (InDeviceType)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				{
					LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating DXGI Factory");
					D3D12Device::Initialize();
				}
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Direct3D 12 Device");
				return new D3D12Device(0);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Vulkan Device");
				return new VulkanDevice(InWindow);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		Device* CreateDevice(_Inout_ GraphicsContext& InContext, _In_ const DeviceType& InDeviceType)
		{
			return CreateDevice(InDeviceType, InContext.GetWindow());
		}

		void DestroyDevice(_Inout_ Device*& InOutDevice)
		{
#ifdef ETERNAL_ENABLE_D3D12
			if (InOutDevice->GetDeviceType() == DeviceType::D3D12)
				D3D12Device::Destroy();
#endif

			delete InOutDevice;
			InOutDevice = nullptr;
		}
	}
}
