#ifndef _VULKAN_ROOT_SIGNATURE_HPP_
#define _VULKAN_ROOT_SIGNATURE_HPP_

#include <cstdint>
#include "Graphics/RootSignature.hpp"

struct VkPipelineLayout_T;
struct VkDescriptorSetLayout_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Sampler;

		class VulkanRootSignature : public RootSignature
		{
		public:
			VulkanRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ const RootSignatureDynamicParameter Parameters[], _In_ uint32_t ParametersCount, _In_ const RootSignature RootSignatures[], _In_ uint32_t RootSignaturesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			VulkanRootSignature(_In_ Device& DeviceObj);
			VkDescriptorSetLayout_T* GetDescriptorSetLayout() { return _DescriptorSetLayout; }
			VkPipelineLayout_T* GetPipelineLayout() { return _PipelineLayout; }

		private:
			VkPipelineLayout_T*			_PipelineLayout			= nullptr;
			VkDescriptorSetLayout_T*	_DescriptorSetLayout	= nullptr;
		};
	}
}

#endif
