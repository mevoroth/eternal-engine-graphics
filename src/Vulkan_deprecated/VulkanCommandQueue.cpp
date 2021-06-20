#include "Vulkan/VulkanCommandQueue.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanContext.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ Device& DeviceObj)
	: _Device(DeviceObj)
{
	//_CommandQueue = static_cast<VulkanDevice&>(DeviceObj).PopVulkanQueue();
}

VulkanCommandQueue::~VulkanCommandQueue()
{
}

void VulkanCommandQueue::Submit(_In_ Context& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
{
	//VulkanContext& VkContext = static_cast<VulkanContext&>(GfxContext);

	//vector<VkCommandBuffer>	VulkanCommandLists;
	//VulkanCommandLists.resize(CommandListsCount);
	//vector<VkSemaphore>&	VulkanSemaphores = VkContext.GetFrameCommandListSemaphores();
	//VulkanSemaphores.resize(CommandListsCount);
	//for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
	//{
	//	VulkanCommandLists[CommandListIndex]	= static_cast<VulkanCommandList*>(CommandLists[CommandListIndex])->GetVulkanCommandList();
	//	VulkanSemaphores[CommandListIndex]		= static_cast<VulkanCommandAllocator*>(CommandLists[CommandListIndex]->GetCommandAllocator())->GetVulkanSemaphore();
	//}
	//
	//VkPipelineStageFlags PipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	//
	//VkSemaphore Semaphore = VkContext.GetFrameSemaphore();

	//VkSubmitInfo SubmitInfo;
	//SubmitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//SubmitInfo.pNext				= nullptr;
	//SubmitInfo.waitSemaphoreCount	= 1;
	//SubmitInfo.pWaitSemaphores		= &Semaphore;
	//SubmitInfo.pWaitDstStageMask	= &PipelineStageFlags;
	//SubmitInfo.commandBufferCount	= (uint32_t)VulkanCommandLists.size();
	//SubmitInfo.pCommandBuffers		= VulkanCommandLists.data();
	//SubmitInfo.signalSemaphoreCount	= (uint32_t)VulkanSemaphores.size();
	//SubmitInfo.pSignalSemaphores	= VulkanSemaphores.data();

	//VkResult Result = vkQueueSubmit(GetVulkanCommandQueue(), 1, &SubmitInfo, static_cast<VulkanFence*>(VkContext.GetFrameFence())->GetVulkanFence());
	//ETERNAL_ASSERT(!Result);
}

VkQueue_T* VulkanCommandQueue::GetVulkanCommandQueue()
{
	return _CommandQueue;
}
