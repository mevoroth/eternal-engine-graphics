#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Android/Vulkan/AndroidVulkanGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{

		AndroidVulkanGraphicsContext::AndroidVulkanGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InAndroidGraphicsContextCreateInformation)
			: AndroidGraphicsContext(InAndroidGraphicsContextCreateInformation)
			, GraphicsContext(InAndroidGraphicsContextCreateInformation, _AndroidOutputDevice)
			, VulkanGraphicsContext(*static_cast<GraphicsContext*>(this))
		{
		}

		AndroidVulkanGraphicsContext::~AndroidVulkanGraphicsContext()
		{
		}

		void AndroidVulkanGraphicsContext::ResetFrameStates()
		{
			GraphicsContext::ResetFrameStates();

			VulkanGraphicsContext::InternalResetFrameStates();
		}
	}
}

#endif
