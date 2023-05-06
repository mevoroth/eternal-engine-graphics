#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VUlkanCommandList.hpp"
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
			, _Device(static_cast<VulkanDevice&>(InDevice))
		{
			int32_t TypeInt = static_cast<int32_t>(Type);
			vk::Device& VkDevice = _Device.GetVulkanDevice();

			QueueFamilyIndicesType QueueFamilyIndices;
			_Device.GetQueueFamilyIndices(QueueFamilyIndices);
	
			uint32_t QueueIndices[] = {
				_Device.GetQueueIndexGraphics(),
				_Device.GetQueueIndexCompute(),
				_Device.GetQueueIndexCopy()
			};

			_QueueFamilyIndex	= QueueFamilyIndices[TypeInt];
			_QueueIndex			= QueueIndices[TypeInt];

			VkDevice.getQueue(
				_QueueFamilyIndex,
				_QueueIndex,
				&_CommandQueue
			);

			_SubmitCompletionSemaphores.resize(SubmitCount[TypeInt]);
			vk::SemaphoreCreateInfo SemaphoreInfo;
			for (uint32_t SemaphoreIndex = 0; SemaphoreIndex < _SubmitCompletionSemaphores.size(); ++SemaphoreIndex)
			{
				Vulkan::VerifySuccess(
					VkDevice.createSemaphore(
						&SemaphoreInfo,
						nullptr,
						&_SubmitCompletionSemaphores[SemaphoreIndex]
					)
				);
			}
		}

		VulkanCommandQueue::~VulkanCommandQueue()
		{
			vk::Device& VkDevice = _Device.GetVulkanDevice();
			
			for (uint32_t SemaphoreIndex = 0; SemaphoreIndex < _SubmitCompletionSemaphores.size(); ++SemaphoreIndex)
				VkDevice.destroySemaphore(_SubmitCompletionSemaphores[SemaphoreIndex]);
		}

		void VulkanCommandQueue::SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext)
		{
			using namespace Eternal::Graphics::Vulkan;

			CommandQueue::SubmitCommandLists(InCommandLists, InCommandListsCount, InContext);

			vk::Semaphore* CurrentFrameSemaphore = InContext ? &static_cast<VulkanGraphicsContext*>(InContext)->GetCurrentFrameSemaphore() : nullptr;

			vk::PipelineStageFlags FrameSemaphoreWaitDestStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

			std::array<vk::CommandBuffer, MaxCommandListsPerSubmission> VulkanCommandLists;
			VulkanCommandLists.fill(vk::CommandBuffer());
			for (uint32_t CommandListIndex = 0; CommandListIndex < InCommandListsCount; ++CommandListIndex)
				VulkanCommandLists[CommandListIndex] = static_cast<VulkanCommandList*>(InCommandLists[CommandListIndex])->GetVulkanCommandBuffer();

			vk::Semaphore* SubmitSemaphore = nullptr;
			if (_SubmitCount < _SubmitCompletionSemaphores.size())
			{
				SubmitSemaphore = &_SubmitCompletionSemaphores[_SubmitCount++];
			}
			else
			{
				_SubmitCompletionSemaphores.push_back(vk::Semaphore());
				SubmitSemaphore = &_SubmitCompletionSemaphores.back();
				vk::SemaphoreCreateInfo SemaphoreInfo;
				VerifySuccess(
					_Device.GetVulkanDevice().createSemaphore(
						&SemaphoreInfo,
						nullptr,
						SubmitSemaphore
					)
				);
			}

			vk::SubmitInfo QueueSubmitInfo(
				InContext ? 1 : 0,
				InContext ? CurrentFrameSemaphore : nullptr,
				InContext ? &FrameSemaphoreWaitDestStageMask : nullptr,
				InCommandListsCount, VulkanCommandLists.data(),
				1, SubmitSemaphore
			);

			VerifySuccess(
				_CommandQueue.submit(
					1, &QueueSubmitInfo,
					InContext ? static_cast<VulkanFence&>(InContext->GetCurrentFrameFence()).GetVulkanFence() : vk::Fence(nullptr)
				)
			);
		}

		void VulkanCommandQueue::GetSubmitCompletionSemaphoresAndReset(_Out_ vk::Semaphore*& OutSemaphores, uint32_t& OutSemaphoresCount)
		{
			OutSemaphoresCount	= _SubmitCount;
			OutSemaphores		= _SubmitCount > 0 ? _SubmitCompletionSemaphores.data() : nullptr;
			_SubmitCount		= 0;
		}
	}
}
