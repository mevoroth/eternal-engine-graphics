#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		namespace VulkanPrivate
		{
			void GatherVulkanInstanceExtensions(_Inout_ std::vector<const char*>& InOutVulkanInstanceExtensions)
			{
				InOutVulkanInstanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
			}
		}
	}
}

#endif
