#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Graphics/CommandUtils.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		static constexpr uint32_t SubmitCount[static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)] =
		{
			128,
			32,
			32
		};

		VulkanCommandQueue::VulkanCommandQueue(_In_ Device& InDevice, _In_ const CommandType& Type)
			: CommandQueue(Type)
		{
			int32_t TypeInt = static_cast<int32_t>(Type);
			VulkanDevice& InVulkanDevice = static_cast<VulkanDevice&>(InDevice);
			vk::Device& VkDevice = InVulkanDevice.GetVulkanDevice();

			QueueFamilyIndicesType QueueFamilyIndices;
			InVulkanDevice.GetQueueFamilyIndices(QueueFamilyIndices);
	
			uint32_t QueueIndices[] = {
				InVulkanDevice.GetQueueIndexGraphics(),
				InVulkanDevice.GetQueueIndexCompute(),
				InVulkanDevice.GetQueueIndexCopy()
			};

			_QueueFamilyIndex	= QueueFamilyIndices[TypeInt];
			_QueueIndex			= QueueIndices[TypeInt];

			VkDevice.getQueue(
				_QueueFamilyIndex,
				_QueueIndex,
				&_CommandQueue
			);
		}

		VulkanCommandQueue::~VulkanCommandQueue()
		{
		}

		void VulkanCommandQueue::SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext)
		{
			using namespace Eternal::Graphics::Vulkan;

			CommandQueue::SubmitCommandLists(InCommandLists, InCommandListsCount, InContext);

			VulkanGraphicsContext* InVulkanGraphicsContext = VulkanGraphicsContextCast(InContext);

			vk::PipelineStageFlags FrameSemaphoreWaitDestStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

			std::array<vk::CommandBuffer, MaxCommandListsPerSubmission> VulkanCommandLists;
			VulkanCommandLists.fill(vk::CommandBuffer());
			for (uint32_t CommandListIndex = 0; CommandListIndex < InCommandListsCount; ++CommandListIndex)
				VulkanCommandLists[CommandListIndex] = static_cast<VulkanCommandList*>(InCommandLists[CommandListIndex])->GetVulkanCommandBuffer();

			vk::SubmitInfo QueueSubmitInfo(
				InContext ? 1u : 0u,
				InContext ? &InVulkanGraphicsContext->GetCurrentFrameSemaphore() : nullptr,
				InContext ? &FrameSemaphoreWaitDestStageMask : nullptr,
				InCommandListsCount, VulkanCommandLists.data(),
				1u, &InVulkanGraphicsContext->GetCurrentSubmitSemaphore()
			);

			VerifySuccess(
				_CommandQueue.submit(
					1u, &QueueSubmitInfo,
					InContext ? static_cast<VulkanFence&>(InContext->GetCurrentFrameFence()).GetVulkanFence() : static_cast<vk::Fence>(nullptr)
				)
			);
		}
	}
}

#endif
