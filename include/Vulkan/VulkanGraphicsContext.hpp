#pragma once

#include "NextGenGraphics/Context.hpp"
#include <array>
#include <vulkan/vulkan.hpp>

namespace Eternal
{
	namespace Graphics
	{
		class VulkanGraphicsContext : public GraphicsContext
		{
		public:
			VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);

			virtual void UpdateGraphicsContext() override;

			vk::Semaphore GetCurrentFrameSemaphore() { return _AcquireFrameSemaphores[CurrentFrameIndex]; }
			vk::Semaphore GetSubmitCompletionSemaphore() { return _SubmitCompletionSemaphore; }

		private:

			std::array<vk::Semaphore, FrameBufferingCount> _AcquireFrameSemaphores;
			vk::Semaphore _SubmitCompletionSemaphore;
			int32_t CurrentFrameIndex = 0;
		};
	}
}
