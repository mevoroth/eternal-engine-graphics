#include "Graphics/DeviceFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullDevice.hpp"
#include "Proxy/ProxyDevice.hpp"
#include "d3d12/D3D12WindowsDevice.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/GraphicsContext.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/DeviceFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		void PreInitializeDevice(_Inout_ GraphicsContext& InOutContext, _In_ const DeviceType& InDeviceType)
		{
#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS
			if (InDeviceType == DeviceType::DEVICE_TYPE_D3D12)
			{
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Initializing D3D12Shader");
				D3D12Shader::Initialize(InOutContext);
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Initializing DXGI Factory");
				D3D12WindowsDevice::Initialize();
			}
#endif
#if ETERNAL_USE_PRIVATE
			PreInitializeDevicePrivate(InOutContext, InDeviceType);
#endif
		}

		Device* CreateDevice(_Inout_ GraphicsContext& InOutContext, _In_ const DeviceType& InDeviceType)
		{
			PreInitializeDevice(InOutContext, InDeviceType);
			switch (InDeviceType)
			{
			case DeviceType::DEVICE_TYPE_NULL:
				return new NullDevice();

			case DeviceType::DEVICE_TYPE_PROXY:
				return new ProxyDevice(InOutContext);

#if ETERNAL_ENABLE_D3D12 && !ETERNAL_USE_PRIVATE
			case DeviceType::DEVICE_TYPE_D3D12:
			{
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Direct3D 12 Device");
				return new D3D12WindowsDevice(0);
			}
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
			{
				LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Vulkan Device");
				return new VulkanDevice(InOutContext.GetOutputDevice());
			}
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateDevicePrivate(InOutContext, InDeviceType);
#endif
				break;
			}

			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyDevice(_Inout_ Device*& InOutDevice)
		{
			PreDestroyDevice(InOutDevice->GetDeviceType());

			delete InOutDevice;
			InOutDevice = nullptr;
		}

		void PreDestroyDevice(_In_ const DeviceType& InDeviceType)
		{
#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS
			if (InDeviceType == DeviceType::DEVICE_TYPE_D3D12)
			{
				D3D12WindowsDevice::Destroy();
				D3D12Shader::Destroy();
			}
#endif
#if ETERNAL_USE_PRIVATE
			PreDestroyDevicePrivate(InDeviceType);
#endif
		}
	}
}
