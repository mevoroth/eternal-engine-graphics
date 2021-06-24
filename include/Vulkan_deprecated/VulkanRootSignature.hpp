#ifndef _VULKAN_ROOT_SIGNATURE_HPP_
#define _VULKAN_ROOT_SIGNATURE_HPP_

#include "Graphics_deprecated/RootSignature.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class Sampler;

		class VulkanRootSignature : public RootSignature
		{
		public:
			VulkanRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			//VulkanRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, /*_In_ const RootSignatureDynamicParameter Parameters[], _In_ uint32_t ParametersCount, _In_ RootSignature* RootSignatures[], _In_ uint32_t RootSignaturesCount,*/ _In_ const RootSignatureAccess& RootSignatureAccessObj);
			VulkanRootSignature(_In_ Device& DeviceObj);
			virtual ~VulkanRootSignature();

			vk::PipelineLayout&	GetPipelineLayout()			{ return _PipelineLayout; }
			uint32_t			GetParametersCount() const	{ return _ParametersCount; }
			const vector<vk::DescriptorSetLayout>& GetDescriptorSetLayouts() const { return _DescriptorSetLayouts; }

		private:
			Device&				_Device;
			vk::PipelineLayout	_PipelineLayout;
			vector<vk::DescriptorSetLayout>	_DescriptorSetLayouts;
			uint32_t						_ParametersCount	= 0u;
		};
	}
}

#endif
