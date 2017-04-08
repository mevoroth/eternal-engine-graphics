#ifndef _VULKAN_DESCRIPTOR_HPP_
#define _VULKAN_DESCRIPTOR_HPP_

#include <cstdint>
#include <vector>
#include "Graphics/DescriptorHeap.hpp"

struct VkDescriptorPool_T;
struct VkDescriptorSetLayout_T;
struct VkDescriptorSet_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class RootSignature;
		enum RootSignatureDynamicParameterType;
		enum RootSignatureAccess;

		class VulkanDescriptorHeap : public DescriptorHeap
		{
		public:
			VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			~VulkanDescriptorHeap();

			VkDescriptorSetLayout_T*& GetDescriptorSetLayout() { return _DescriptorSetLayout; }
			VkDescriptorSet_T* Pop();
			void Push(_In_ VkDescriptorSet_T* Handle);

		private:
			vector<VkDescriptorSet_T*>	_ResourcesPool;

			Device& _Device;
			VkDescriptorPool_T*			_DescriptorPool			= nullptr;
			VkDescriptorSetLayout_T*	_DescriptorSetLayout	= nullptr;
		};
	}
}

#endif
