#include "Graphics/DeviceFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Device* CreateDevice(_Inout_ GraphicsContext& InOutContext, _In_ const DeviceType& InDeviceType)
		{
			switch (InDeviceType)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
			{
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Initializing D3D12Shader");
				D3D12Shader::Initialize(InOutContext);
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Initializing DXGI Factory");
				D3D12Device::Initialize();
			}
			LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Direct3D 12 Device");
			return new D3D12Device(0);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Vulkan Device");
				return new VulkanDevice(InOutContext.GetWindow());
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		void DestroyDevice(_Inout_ Device*& InOutDevice)
		{
#ifdef ETERNAL_ENABLE_D3D12
			if (InOutDevice->GetDeviceType() == DeviceType::D3D12)
			{
				D3D12Device::Destroy();
				D3D12Shader::Destroy();
			}
#endif

			delete InOutDevice;
			InOutDevice = nullptr;
		}
	}
}
