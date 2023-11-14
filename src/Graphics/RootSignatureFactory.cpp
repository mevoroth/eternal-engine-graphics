#include "Graphics/RootSignatureFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/RootSignatureFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		RootSignature* CreateRootSignature(_In_ GraphicsContext& InContext, _In_ bool InIsLocalRootSignature)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12RootSignature(InContext.GetDevice(), InIsLocalRootSignature);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanRootSignature(InContext);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateRootSignaturePrivate(InContext, InIsLocalRootSignature);
#endif
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		RootSignature* CreateRootSignature(_In_ GraphicsContext& InContext, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12RootSignature(InContext.GetDevice(), InRootSignatureCreateInformation);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanRootSignature(InContext, InRootSignatureCreateInformation);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateRootSignaturePrivate(InContext, InRootSignatureCreateInformation);
#endif
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyRootSignature(_Inout_ RootSignature*& InOutRootSignature)
		{
			delete InOutRootSignature;
			InOutRootSignature = nullptr;
		}
	}
}
