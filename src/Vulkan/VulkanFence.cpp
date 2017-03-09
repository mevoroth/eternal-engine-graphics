#include "Vulkan/VulkanFence.hpp"

#include "Macros/Macros.hpp"

#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanCommandList.hpp"

using namespace Eternal::Graphics;

VulkanFence::VulkanFence(_In_ VulkanDevice& DeviceObj, _In_ uint32_t SimultaneousResourcesCount)
	: _Device(DeviceObj)
{
	ETERNAL_ASSERT(SimultaneousResourcesCount > 0);

	_Fences.resize(SimultaneousResourcesCount);

	for (uint32_t ResourceIndex = 0; ResourceIndex < SimultaneousResourcesCount; ++ResourceIndex)
	{
		VkFenceCreateInfo FenceInfo;
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.pNext = nullptr;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkResult Result = vkCreateFence(DeviceObj.GetDevice(), &FenceInfo, nullptr, &_Fences[ResourceIndex]);
		ETERNAL_ASSERT(!Result);
	}

	VkResult Result = vkResetFences(DeviceObj.GetDevice(), _Fences.size(), _Fences.data());
	ETERNAL_ASSERT(!Result);
}

VulkanFence::~VulkanFence()
{
	for (uint32_t ResourceIndex = 0; ResourceIndex < _Fences.size(); ++ResourceIndex)
	{
		vkDestroyFence(_Device.GetDevice(), _Fences[ResourceIndex], nullptr);
	}
}

void VulkanFence::Signal(_In_ VulkanSwapChain& SwapChainObj, _In_ VulkanCommandQueue& CommandQueueObj, _In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount)
{
	vector<VkCommandBuffer> VulkanCommandLists;
	for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
	{
		VulkanCommandLists.push_back(CommandLists[CommandListIndex].GetVulkanCommandList());
	}

	VkPipelineStageFlags PipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo SubmitInfo;
	SubmitInfo.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.pNext					= nullptr;
	SubmitInfo.waitSemaphoreCount		= 1;
	SubmitInfo.pWaitSemaphores			= &SwapChainObj.GetAcquireSemaphore(_FenceIndex);
	SubmitInfo.pWaitDstStageMask		= &PipelineStageFlags;
	SubmitInfo.commandBufferCount		= VulkanCommandLists.size();
	SubmitInfo.pCommandBuffers			= VulkanCommandLists.data();
	SubmitInfo.signalSemaphoreCount		= 1;
	SubmitInfo.pSignalSemaphores		= &CommandQueueObj.GetCompletedSemaphore(_FenceIndex);

	VkResult Result = vkQueueSubmit(CommandQueueObj.GetCommandQueue(), 1, &SubmitInfo, /*_Fences[_FenceIndex]*/nullptr);
	ETERNAL_ASSERT(!Result);
}

void VulkanFence::Wait(_In_ VulkanDevice& DeviceObj)
{
	VkResult Result;
	//Result = vkGetFenceStatus(DeviceObj.GetDevice(), _Fences[_FenceIndex]);
	//if (Result == VK_SUCCESS)
	//{
		Result = vkWaitForFences(DeviceObj.GetDevice(), 1, &_Fences[_FenceIndex], VK_TRUE, UINT64_MAX);
		ETERNAL_ASSERT(!Result);
	//}
}

void VulkanFence::Reset(_In_ VulkanDevice& DeviceObj)
{
	_FenceIndex = (_FenceIndex + 1) % _Fences.size();
	VkResult Result = vkResetFences(DeviceObj.GetDevice(), 1, &_Fences[_FenceIndex]);
	ETERNAL_ASSERT(!Result);
}

VkFence_T* VulkanFence::GetFence(/*_In_ uint32_t FenceIndex*/)
{
	return _Fences[_FenceIndex];
}
