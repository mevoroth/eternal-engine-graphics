#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanUtils.hpp"

#include "Android/Vulkan/AndroidVulkanGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		namespace Vulkan
		{
			VulkanGraphicsContext& VulkanGraphicsContextCast(_In_ GraphicsContext& InContext)
			{
				return static_cast<AndroidVulkanGraphicsContext&>(InContext);
			}

			VulkanGraphicsContext* VulkanGraphicsContextCast(_In_ GraphicsContext* InContext)
			{
				return static_cast<AndroidVulkanGraphicsContext*>(InContext);
			}
		}
	}
}

#endif
