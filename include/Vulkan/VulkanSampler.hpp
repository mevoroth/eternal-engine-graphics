#ifndef _VULKAN_SAMPLER_HPP_
#define _VULKAN_SAMPLER_HPP_

#include "Graphics/Sampler.hpp"

struct VkSampler_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanSampler : public Sampler
		{
		public:
			VkSampler_T*& GetVulkanSampler() { return _Sampler; }

		private:
			VkSampler_T* _Sampler = nullptr;
		};
	}
}

#endif
