#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VUlkanCommandList.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanHeader.hpp"

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ Device& InDevice, _In_ const CommandType& Type)
	: CommandQueue(Type)
	, _Device(InDevice)
{
	VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(InDevice);
	vk::Device& VkDevice = VulkanDeviceObj.GetVulkanDevice();

	uint32_t QueueFamilyIndices[] = {
		VulkanDeviceObj.GetQueueFamilyIndexGraphics(),
		VulkanDeviceObj.GetQueueFamilyIndexCompute(),
		VulkanDeviceObj.GetQueueFamilyIndexCopy()
	};
	
	uint32_t QueueIndices[] = {
		VulkanDeviceObj.GetQueueIndexGraphics(),
		VulkanDeviceObj.GetQueueIndexCompute(),
		VulkanDeviceObj.GetQueueIndexCopy()
	};

	_QueueFamilyIndex	= QueueFamilyIndices[static_cast<int32_t>(Type)];
	_QueueIndex			= QueueIndices[static_cast<int32_t>(Type)];

	VkDevice.getQueue(
		_QueueFamilyIndex,
		_QueueIndex,
		&_CommandQueue
	);

	vk::SemaphoreCreateInfo SemaphoreInfo;
	Vulkan::VerifySuccess(
		VkDevice.createSemaphore(&SemaphoreInfo, nullptr, &_SubmitCompletionSemaphore)
	);
}

VulkanCommandQueue::~VulkanCommandQueue()
{
	vk::Device& VkDevice = static_cast<VulkanDevice&>(_Device).GetVulkanDevice();
	VkDevice.destroySemaphore(_SubmitCompletionSemaphore);
}

void VulkanCommandQueue::SubmitCommandLists(_In_ GraphicsContext& Context, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
{
	VulkanGraphicsContext& VulkanGfxContext = static_cast<VulkanGraphicsContext&>(Context);
	vk::Semaphore CurrentFrameSemaphore = VulkanGfxContext.GetCurrentFrameSemaphore();

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
		1, &_SubmitCompletionSemaphore
	);

	VulkanFence& VkFence = static_cast<VulkanFence&>(Context.GetSubmitFence());

	Vulkan::VerifySuccess(
		_CommandQueue.submit(1, &SubmitInfo, VkFence.GetVulkanFence())
	);

	_HasSubmittedCommandLists = VulkanCommandLists.size() > 0;
}

vk::Semaphore* VulkanCommandQueue::GetSubmitCompletionSemaphoreAndReset()
{
	vk::Semaphore* OutSemaphore = nullptr;
	if (_HasSubmittedCommandLists)
	{
		OutSemaphore = &_SubmitCompletionSemaphore;
	}
	_HasSubmittedCommandLists = false;
	return OutSemaphore;
}
