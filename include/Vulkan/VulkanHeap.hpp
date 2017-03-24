#ifndef _VULKAN_HEAP_HPP_
#define _VULKAN_HEAP_HPP_

#include "Graphics/Heap.hpp"

struct VkDeviceMemory_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanHeap : public Heap
		{
		public:
			VulkanHeap(_In_ Device& DeviceObj, _In_ size_t Size, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached);
			~VulkanHeap();

		private:
			Device&				_Device;
			VkDeviceMemory_T*	_DeviceMemory = nullptr;
		};
	}
}

#endif
