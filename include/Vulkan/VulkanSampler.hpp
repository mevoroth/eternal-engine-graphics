#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Graphics/Sampler.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;
		class GraphicsContext;

		class VulkanSampler final : public Sampler
		{
		public:
			VulkanSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation);
			~VulkanSampler();
			
			const vk::Sampler& GetVulkanSampler() const { return _Sampler; }

		private:
			VulkanDevice&	_Device;
			vk::Sampler		_Sampler;
		};
	}
}

#endif
