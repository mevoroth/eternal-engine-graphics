#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		class VulkanGraphicsContext : public GraphicsContext
		{
		public:
			VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			~VulkanGraphicsContext();

			virtual void UpdateGraphicsContext() override;

			vk::Semaphore& GetCurrentFrameSemaphore() { return _AcquireFrameSemaphores[GetCurrentFrameIndex()]; }

		private:

			std::array<vk::Semaphore, FrameBufferingCount> _AcquireFrameSemaphores;
		};
	}
}
