#include "Vulkan/VulkanCommandQueue.hpp"

#include <vector>
#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
<<<<<<< HEAD
<<<<<<< Updated upstream
=======
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanFence.hpp"
>>>>>>> Stashed changes
=======
#include "Vulkan/VulkanCommandList.hpp"
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ VulkanDevice& Device)
	: _Device(Device)
{
	vkGetDeviceQueue(_Device.GetDevice(), 0, 0, &_CommandQueue);

	VkCommandPoolCreateInfo CommandPoolInfo;
	CommandPoolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolInfo.pNext				= nullptr;
	CommandPoolInfo.flags				= 0;
	CommandPoolInfo.queueFamilyIndex	= Device.GetQueueFamilyIndex();

	VkResult Result = vkCreateCommandPool(Device.GetDevice(), &CommandPoolInfo, nullptr, &_CommandPool);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandQueue::~VulkanCommandQueue()
{
	vkDestroyCommandPool(_Device.GetDevice(), _CommandPool, nullptr);
	_CommandPool = nullptr;
}

void VulkanCommandQueue::Reset(_In_ uint32_t FrameIndex)
{
	VkResult Result = vkResetCommandPool(_Device.GetDevice(), _CommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	ETERNAL_ASSERT(!Result);
}

<<<<<<< HEAD
<<<<<<< Updated upstream
=======
void VulkanCommandQueue::Flush(_In_ VulkanFence& FenceObj, _In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount)
=======
void VulkanCommandQueue::Flush(_In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount)
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce
{
	vector<VkCommandBuffer> VulkanCommandLists;
	for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
	{
		VulkanCommandLists.push_back(CommandLists[CommandListIndex].GetVulkanCommandList());
	}

	VkSubmitInfo SubmitInfo;
<<<<<<< HEAD
	SubmitInfo.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.pNext					= nullptr;
	SubmitInfo.waitSemaphoreCount		= 0;
	SubmitInfo.pWaitSemaphores			= nullptr;
	SubmitInfo.pWaitDstStageMask		= nullptr;
	SubmitInfo.commandBufferCount		= CommandListsCount;
	SubmitInfo.pCommandBuffers			= VulkanCommandLists.data();
	SubmitInfo.signalSemaphoreCount		= 0;
	SubmitInfo.pSignalSemaphores		= nullptr;

	// FIX THIS
	VkResult Result = vkQueueSubmit(_CommandQueue, 1, &SubmitInfo, FenceObj.GetFence());
	ETERNAL_ASSERT(!Result);
>>>>>>> Stashed changes
=======
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.pNext = nullptr;
	SubmitInfo.waitSemaphoreCount = 0;
	SubmitInfo.pWaitSemaphores = nullptr;
	SubmitInfo.commandBufferCount = CommandListsCount;
	SubmitInfo.pCommandBuffers = VulkanCommandLists.data();
	SubmitInfo.signalSemaphoreCount = 0;
	SubmitInfo.pSignalSemaphores = nullptr;

	// FIX THIS
	VkResult Result = vkQueueSubmit(_CommandQueue, 1, &SubmitInfo, nullptr);
	ETERNAL_ASSERT(!Result);
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce
}
