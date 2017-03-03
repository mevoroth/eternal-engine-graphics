#ifndef _VULKAN_COMMAND_LIST_HPP_
#define _VULKAN_COMMAND_LIST_HPP_

#include <cstdint>

struct VkCommandBuffer_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;
		class VulkanCommandQueue;

		class VulkanCommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& Device, _In_ VulkanCommandQueue& CommandQueue);
			~VulkanCommandList();

			void Begin();
			void DrawPrimitive(_In_ uint32_t PrimitiveCount);
			void End();

		private:
			VulkanDevice& _Device;
			VulkanCommandQueue& _CommandQueue;
			VkCommandBuffer_T* _CommandBuffer = nullptr;
		};
	}
}

#endif
