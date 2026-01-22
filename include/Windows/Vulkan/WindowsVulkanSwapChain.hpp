#pragma once

#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanSwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class WindowsVulkanSwapChain final : public VulkanSwapChain
		{
		public:

			WindowsVulkanSwapChain(_In_ GraphicsContext& InContext);

		protected:

			virtual void CreateSurface(_In_ GraphicsContext& InContext) override final;

		};
	}
}

#endif
