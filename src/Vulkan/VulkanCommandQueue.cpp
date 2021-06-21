#include "Vulkan/VulkanCommandQueue.hpp"
#include <vulkan/vulkan.hpp>
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VUlkanCommandList.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ Device& DeviceObj, _In_ const CommandType& Type)
	: CommandQueue(DeviceObj, Type)
{
}

VulkanCommandQueue::~VulkanCommandQueue()
{
}

void VulkanCommandQueue::SubmitCommandLists(_In_ GraphicsContext& Context, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
{
	
	//vk::SubmitInfo SubmitInfo(
	//	1, &CurrentSemaphore,
	//	&WaitDestStageMask,
	//	HasRequests ? 2 : 1, HasRequests ? SubmitCmdList : &VulkanCommandBuffer,
	//	1, &ReleaseSemaphore
	//);
	//Vulkan::VerifySuccess(VulkanQueue.submit(1, &SubmitInfo, SubmitFence));

	//SubmitInfo(uint32_t                                         waitSemaphoreCount_ = {},
	//	const VULKAN_HPP_NAMESPACE::Semaphore * pWaitSemaphores_ = {},
	//	const VULKAN_HPP_NAMESPACE::PipelineStageFlags * pWaitDstStageMask_ = {},
	//	uint32_t                                         commandBufferCount_ = {},
	//	const VULKAN_HPP_NAMESPACE::CommandBuffer * pCommandBuffers_ = {},
	//	uint32_t                                         signalSemaphoreCount_ = {},
	//	const VULKAN_HPP_NAMESPACE::Semaphore * pSignalSemaphores_ = {}) VULKAN_HPP_NOEXCEPT

	VulkanGraphicsContext& VulkanGfxContext = static_cast<VulkanGraphicsContext&>(Context);
	vk::Semaphore CurrentFrameSemaphore = VulkanGfxContext.GetCurrentFrameSemaphore();
	vk::Semaphore SubmitCompletionSemaphore = VulkanGfxContext.GetSubmitCompletionSemaphore();

	vk::PipelineStageFlags WaitDestStageMask = vk::PipelineStageFlagBits::eAllGraphics;

	vector<vk::CommandBuffer> VulkanCommandLists;
	VulkanCommandLists.resize(CommandListsCount);
	for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
	{
		VulkanCommandLists[CommandListIndex] = static_cast<VulkanCommandList*>(CommandLists[CommandListIndex])->GetVulkanCommandBuffer();
	}

	vk::SubmitInfo SubmitInfo(
		1, &CurrentFrameSemaphore,
		&WaitDestStageMask,
		CommandListsCount, VulkanCommandLists.data(),
		1, &SubmitCompletionSemaphore
	);

	//Vulkan::VerifySuccess(
	//	_CommandQueue.submit(1, &SubmitInfo, )
	//);
}
