#ifndef _VULKAN_ROOT_SIGNATURE_HPP_
#define _VULKAN_ROOT_SIGNATURE_HPP_

#include <cstdint>
#include <vector>
#include "Graphics/RootSignature.hpp"

enum VkDescriptorType;
struct VkPipelineLayout_T;
struct VkDescriptorSetLayout_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class Sampler;
		class DescriptorHeap;

		extern const VkDescriptorType VULKAN_DESCRIPTOR_TYPES[];

		class VulkanRootSignature : public RootSignature
		{
		public:
			VulkanRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			//VulkanRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, /*_In_ const RootSignatureDynamicParameter Parameters[], _In_ uint32_t ParametersCount, _In_ RootSignature* RootSignatures[], _In_ uint32_t RootSignaturesCount,*/ _In_ const RootSignatureAccess& RootSignatureAccessObj);
			VulkanRootSignature(_In_ Device& DeviceObj);
			virtual ~VulkanRootSignature();

			VkPipelineLayout_T*	GetPipelineLayout()			{ return _PipelineLayout; }
			uint32_t			GetParametersCount() const	{ return _ParametersCount; }

		private:
			Device&				_Device;
			VkPipelineLayout_T*	_PipelineLayout		= nullptr;
			uint32_t			_ParametersCount	= 0u;
		};
	}
}

#endif
