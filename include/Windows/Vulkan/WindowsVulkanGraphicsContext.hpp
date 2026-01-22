#pragma once

#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

#include "Windows/WindowsGraphicsContext.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Bit;

		class VulkanDescriptorTable;

		class WindowsVulkanGraphicsContext final
			: public WindowsGraphicsContext
			, public GraphicsContext
			, public VulkanGraphicsContext
		{
		public:

			WindowsVulkanGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation);
			~WindowsVulkanGraphicsContext();

			virtual void ResetFrameStates() override final;

		};
	}
}

#endif
