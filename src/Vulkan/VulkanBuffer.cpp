#include "Vulkan/VulkanBuffer.hpp"

#include "Macros/Macros.hpp"

#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanBuffer::VulkanBuffer(_In_ VulkanDevice& Device)
{
	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = nullptr;
	BufferCreateInfo.flags = 0;
	BufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	BufferCreateInfo.size = sizeof(float) * 1024;
	BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	BufferCreateInfo.queueFamilyIndexCount = 0;
	BufferCreateInfo.pQueueFamilyIndices = nullptr;

	VkResult Result = vkCreateBuffer(Device.GetDevice(), &BufferCreateInfo, nullptr, &_Buffer);
	ETERNAL_ASSERT(!Result);
	
	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(Device.GetDevice(), _Buffer, &MemoryRequirements);

	VkMemoryAllocateInfo MemoryAllocateInfo;
	MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocateInfo.pNext = nullptr;
	//MemoryAllocateInfo.memoryTypeIndex = 
}

void* VulkanBuffer::Map(_In_ VulkanDevice& Device)
{
	//VkDeviceMemory
	//vkMapMemory(Device.GetDevice(), )
	return nullptr;
}

void VulkanBuffer::Unmap()
{
}
