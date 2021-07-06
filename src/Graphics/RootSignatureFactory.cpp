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

			case DeviceType::VULKAN:
				return new VulkanRootSignature(InContext);

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

			case DeviceType::VULKAN:
				return new VulkanRootSignature(InContext, InRootSignatureCreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
