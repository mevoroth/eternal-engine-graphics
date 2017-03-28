#ifndef _VULKAN_DESCRIPTOR_HPP_
#define _VULKAN_DESCRIPTOR_HPP_

#include <cstdint>
#include "Graphics/DescriptorHeap.hpp"

struct VkDescriptorPool_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		enum RootSignatureDynamicParameterType;

		class VulkanDescriptorHeap : public DescriptorHeap
		{
		public:
			VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount);

		private:
			VkDescriptorPool_T* _DescriptorPool = nullptr;
		};
	}
}

#endif
