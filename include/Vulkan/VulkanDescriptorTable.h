#ifndef _VULKAN_DESCRIPTOR_TABLE_HPP_
#define _VULKAN_DESCRIPTOR_TABLE_HPP_

#include <vector>
#include "Graphics/DescriptorTable.h"

struct VkDescriptorSet_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class VulkanDescriptorHeap;

		class VulkanDescriptorTable : public DescriptorTable
		{
		public:
			VulkanDescriptorTable(_In_ VulkanDescriptorHeap& DescriptorHeapObj, _In_ uint32_t ResourcesCount);

			VkDescriptorSet_T*& GetDescriptorSet();

			virtual void SetResource(_In_ uint32_t Slot, _In_ View* ViewObj) override;
			virtual void SetResource(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void Update(_In_ Device& DeviceObj) override;

		private:
			VulkanDescriptorHeap&	_DescriptorHeap;
			vector<void*>			_Resources;
		};
	}
}

#endif
