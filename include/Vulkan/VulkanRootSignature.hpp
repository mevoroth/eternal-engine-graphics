#ifndef _VULKAN_ROOT_SIGNATURE_HPP_
#define _VULKAN_ROOT_SIGNATURE_HPP_

#include "Graphics/RootSignature.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;

		class VulkanRootSignature : public RootSignature
		{
		public:
			VulkanRootSignature(_In_ Device& InDevice);
			VulkanRootSignature(_In_ Device& InDevice, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation);
			virtual ~VulkanRootSignature();

			inline vk::PipelineLayout& GetPipelineLayout() { return _PipelineLayout; }

		private:
			Device&					_Device;
			vk::PipelineLayout		_PipelineLayout;
			vk::DescriptorSetLayout	_DescriptorSetLayout;

		//public:
		//	VulkanRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
		//	VulkanRootSignature(_In_ Device& DeviceObj);
		//	virtual ~VulkanRootSignature();

		//	vk::PipelineLayout&	GetPipelineLayout()			{ return _PipelineLayout; }
		//	uint32_t			GetParametersCount() const	{ return _ParametersCount; }
		//	const vector<vk::DescriptorSetLayout>& GetDescriptorSetLayouts() const { return _DescriptorSetLayouts; }

		//private:
		//	Device&				_Device;
		//	vk::PipelineLayout	_PipelineLayout;
		//	vector<vk::DescriptorSetLayout>	_DescriptorSetLayouts;
		//	uint32_t						_ParametersCount	= 0u;
		};
	}
}

#endif
