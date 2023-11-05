#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanSampler.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanSampler::VulkanSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation)
			: Sampler(InSamplerCreateInformation)
			, _Device(static_cast<VulkanDevice&>(InContext.GetDevice()))
		{
			using namespace Eternal::Graphics::Vulkan;

			vk::SamplerCreateInfo SamplerCreateInfo(
				vk::SamplerCreateFlagBits(),
				(IsAnisotropic() || GetMAGLinear()) ? vk::Filter::eLinear : vk::Filter::eNearest,
				(IsAnisotropic() || GetMINLinear()) ? vk::Filter::eLinear : vk::Filter::eNearest,
				(IsAnisotropic() || GetMIPLinear()) ? vk::SamplerMipmapMode::eLinear : vk::SamplerMipmapMode::eNearest,
				ConvertAddressModeToVulkanSamplerAddressMode(GetU()),
				ConvertAddressModeToVulkanSamplerAddressMode(GetV()),
				ConvertAddressModeToVulkanSamplerAddressMode(GetW()),
				GetMipLODBias(),
				IsAnisotropic(),
				static_cast<float>(GetMaxAnisotropy()),
				IsComparisonEnabled(),
				ConvertComparisonFunctionToVulkanComparisonOperator(GetComparisonFunction()),
				GetMinLOD(),
				GetMaxLOD(),
				ConvertBorderColorToVulkanBorderColor(GetBorderColor()),
				/*unnormalizedCoordinates_=*/ false
			);

			VerifySuccess(
				_Device.GetVulkanDevice().createSampler(
					&SamplerCreateInfo,
					nullptr,
					&_Sampler
				)
			);
		}

		VulkanSampler::~VulkanSampler()
		{
			_Device.GetVulkanDevice().destroySampler(_Sampler);
		}
	}
}

#endif
