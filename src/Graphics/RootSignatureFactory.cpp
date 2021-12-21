#include "Graphics/RootSignatureFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "d3d12/D3D12RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		RootSignature* CreateRootSignature(_In_ GraphicsContext& InContext)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12RootSignature(InContext.GetDevice());
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				return new VulkanRootSignature(InContext);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		RootSignature* CreateRootSignature(_In_ GraphicsContext& InContext, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12RootSignature(InContext.GetDevice(), InRootSignatureCreateInformation);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				return new VulkanRootSignature(InContext, InRootSignatureCreateInformation);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		void DestroyRootSignature(_Inout_ RootSignature*& InOutRootSignature)
		{
			delete InOutRootSignature;
			InOutRootSignature = nullptr;
		}
	}
}
