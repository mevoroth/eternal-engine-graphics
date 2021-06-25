#pragma once

#include "Graphics/Sampler.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class GraphicsContext;

		class VulkanSampler : public Sampler
		{
		public:
			VulkanSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation);
			~VulkanSampler();
			
			vk::Sampler& GetVulkanSampler() { return _Sampler; }

		private:
			Device&		_Device;
			vk::Sampler	_Sampler;
		};
	}
}
