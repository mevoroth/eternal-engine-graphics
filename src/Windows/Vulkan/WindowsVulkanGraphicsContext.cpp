#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

#include "Windows/Vulkan/WindowsVulkanGraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanDescriptorTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;

		WindowsVulkanGraphicsContext::WindowsVulkanGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsCreateInformation)
			: WindowsGraphicsContext(InWindowsGraphicsCreateInformation)
			, GraphicsContext(InWindowsGraphicsCreateInformation, _WindowsOutputDevice)
			, VulkanGraphicsContext(*static_cast<GraphicsContext*>(this))
		{
		}

		WindowsVulkanGraphicsContext::~WindowsVulkanGraphicsContext()
		{
		}

		void WindowsVulkanGraphicsContext::ResetFrameStates()
		{
			GraphicsContext::ResetFrameStates();

			VulkanGraphicsContext::InternalResetFrameStates();
		}
	}
}

#endif
