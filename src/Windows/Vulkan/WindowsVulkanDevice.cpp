#include "Vulkan/VulkanDevice.hpp"

#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

namespace Eternal
{
	namespace Graphics
	{
		namespace VulkanPrivate
		{
			void GatherVulkanInstanceExtensions(_Inout_ std::vector<const char*>& InOutVulkanInstanceExtensions)
			{
				InOutVulkanInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			}
		}
	}
}

#endif
