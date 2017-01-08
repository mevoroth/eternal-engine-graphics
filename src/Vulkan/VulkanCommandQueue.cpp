#include "Vulkan/VulkanCommandQueue.hpp"

#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanCommandQueue::VulkanCommandQueue(_In_ VulkanDevice& Device)
{
	vkGetDeviceQueue(Device.GetDevice(), 0, 0, &_CommandQueue);
}
