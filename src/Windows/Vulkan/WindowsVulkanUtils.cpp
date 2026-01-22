#include "Vulkan/VulkanUtils.hpp"

#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

#include "Windows/Vulkan/WindowsVulkanGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		namespace Vulkan
		{
			VulkanGraphicsContext& VulkanGraphicsContextCast(_In_ GraphicsContext& InContext)
			{
				return static_cast<WindowsVulkanGraphicsContext&>(InContext);
			}

			VulkanGraphicsContext* VulkanGraphicsContextCast(_In_ GraphicsContext* InContext)
			{
				return static_cast<WindowsVulkanGraphicsContext*>(InContext);
			}
		}
	}
}

#endif
