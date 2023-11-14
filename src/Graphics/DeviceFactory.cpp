#include "Graphics/DeviceFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/DeviceFactoryPrivate.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Device* CreateDevice(_Inout_ GraphicsContext& InOutContext, _In_ const DeviceType& InDeviceType)
		{
			switch (InDeviceType)
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
			{
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Initializing D3D12Shader");
				D3D12Shader::Initialize(InOutContext);
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Initializing DXGI Factory");
				D3D12Device::Initialize();
			}
			LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Direct3D 12 Device");
			return new D3D12Device(0);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Vulkan Device");
				return new VulkanDevice(InOutContext.GetOutputDevice());
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateDevicePrivate(InOutContext, InDeviceType);
#endif
			}

			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyDevice(_Inout_ Device*& InOutDevice)
		{
#if ETERNAL_ENABLE_D3D12
			if (InOutDevice->GetDeviceType() == DeviceType::DEVICE_TYPE_D3D12)
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
