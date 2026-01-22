#pragma once

#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanSwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class AndroidVulkanSwapChain final : public VulkanSwapChain
		{
		public:

			AndroidVulkanSwapChain(_In_ GraphicsContext& InContext);

		protected:

			virtual void CreateSurface(_In_ GraphicsContext& InContext) override final;

		};
	}
}

#endif
