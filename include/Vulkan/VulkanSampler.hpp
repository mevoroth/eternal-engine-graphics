#ifndef _VULKAN_SAMPLER_HPP_
#define _VULKAN_SAMPLER_HPP_

#include "Graphics/Sampler.hpp"

struct VkSampler_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanSampler : public Sampler
		{
		public:
			VulkanSampler(_In_ Device& DeviceObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
			VkSampler_T*& GetVulkanSampler() { return _Sampler; }

		private:
			VkSampler_T* _Sampler = nullptr;
		};
	}
}

#endif
