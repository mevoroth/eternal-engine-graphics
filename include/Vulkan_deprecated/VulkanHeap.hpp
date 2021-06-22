#ifndef _VULKAN_HEAP_HPP_
#define _VULKAN_HEAP_HPP_

#include "Graphics_deprecated/Heap.hpp"

enum VkMemoryPropertyFlagBits;
struct VkDeviceMemory_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanHeap : public Heap
		{
		public:
			VulkanHeap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached);
			~VulkanHeap();

			void Initialize(_In_ uint32_t MemoryTypeBitsRequirement, _In_ size_t Size);
			virtual void Initialize(_In_ size_t Size) override;
			virtual bool IsInitialized() const override;

			VkDeviceMemory_T* GetVulkanDeviceMemory() { return _DeviceMemory; }

		private:
			VkDeviceMemory_T*			_DeviceMemory				= nullptr;
			VkMemoryPropertyFlagBits	_MemoryPropertiesFlags		= (VkMemoryPropertyFlagBits)0;
			uint32_t					_MemoryTypeBitsRequirement	= uint32_t(~0);
		};
	}
}

#endif