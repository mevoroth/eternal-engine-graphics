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

			vk::Semaphore* CurrentFrameSemaphore = InContext ? &VulkanGraphicsContextCast(InContext)->GetCurrentFrameSemaphore() : nullptr;

			vk::PipelineStageFlags FrameSemaphoreWaitDestStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

			std::array<vk::CommandBuffer, MaxCommandListsPerSubmission> VulkanCommandLists;
			VulkanCommandLists.fill(vk::CommandBuffer());
			for (uint32_t CommandListIndex = 0; CommandListIndex < InCommandListsCount; ++CommandListIndex)
				VulkanCommandLists[CommandListIndex] = static_cast<VulkanCommandList*>(InCommandLists[CommandListIndex])->GetVulkanCommandBuffer();

			_SubmitCompletionSemaphores.insert(
				_SubmitCompletionSemaphores.end(),
				_InactiveSubmitCompletionSemaphores.begin(),
				_InactiveSubmitCompletionSemaphores.end()
			);

			_InactiveSubmitCompletionSemaphores.clear();

			if (_InactiveSubmitCompletionSemaphores.size() > 0)
			{
				vk::Device& VkDevice = static_cast<VulkanDevice&>(InContext->GetDevice()).GetVulkanDevice();

				vector<uint64_t> SemaphoreValues;
				SemaphoreValues.resize(_InactiveSubmitCompletionSemaphores.size());
				for (uint32_t SemaphoreIndex = 0; SemaphoreIndex < _InactiveSubmitCompletionSemaphores.size(); ++SemaphoreIndex)
					SemaphoreValues[SemaphoreIndex] = 1ull;

				vk::SemaphoreWaitInfo WaitInfo(
					vk::SemaphoreWaitFlagBits::eAny,
					_InactiveSubmitCompletionSemaphores.size(),
					_InactiveSubmitCompletionSemaphores.data(),
					SemaphoreValues.data()
				);
				VerifySuccess(
					VkDevice.waitSemaphores(
						&WaitInfo,
						UINT64_MAX
					)
				);
			}

			vk::Semaphore* SubmitSemaphore = nullptr;
			if (_SubmitCompletionSemaphores.size() > 0)
			{
				_ActiveSubmitCompletionSemaphores.push_back(_SubmitCompletionSemaphores.back());
				_SubmitCompletionSemaphores.pop_back();
				SubmitSemaphore = &_ActiveSubmitCompletionSemaphores.back();
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

		void VulkanCommandQueue::AcquireSubmitCompletionSemaphores(_Out_ vector<vk::Semaphore>& OutSemaphores)
		{
			OutSemaphores = std::move(_ActiveSubmitCompletionSemaphores);
		}

		void VulkanCommandQueue::ReleaseSubmitCompletionSemaphores(_In_ vector<vk::Semaphore>&& InSemaphores)
		{
			_InactiveSubmitCompletionSemaphores.insert(
				_InactiveSubmitCompletionSemaphores.end(),
				InSemaphores.begin(),
				InSemaphores.end()
			);
		}
	}
}

#endif
