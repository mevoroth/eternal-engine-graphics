#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanWindowsGraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanDescriptorTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;

		VulkanWindowsGraphicsContext::VulkanWindowsGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsCreateInformation)
			: WindowsGraphicsContext(InWindowsGraphicsCreateInformation)
			, GraphicsContext(InWindowsGraphicsCreateInformation, _WindowsOutputDevice)
			, VulkanGraphicsContext(*static_cast<GraphicsContext*>(this))
		{
		}

		VulkanWindowsGraphicsContext::~VulkanWindowsGraphicsContext()
		{
		}

		void VulkanWindowsGraphicsContext::ResetFrameStates()
		{
			GraphicsContext::ResetFrameStates();

			VulkanGraphicsContext::ResetFrameStates();
		}
	}
}

#endif
