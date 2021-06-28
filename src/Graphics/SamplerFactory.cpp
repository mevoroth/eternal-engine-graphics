#include "Graphics/SamplerFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "Vulkan/VulkanSampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Sampler* CreateSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Sampler(InContext, InSamplerCreateInformation);
#endif
			case DeviceType::VULKAN:
				return new VulkanSampler(InContext, InSamplerCreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
