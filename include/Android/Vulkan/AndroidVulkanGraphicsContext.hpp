#pragma once

#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Android/AndroidGraphicsContext.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Bit;

		class VulkanDescriptorTable;

		class AndroidVulkanGraphicsContext final
			: public AndroidGraphicsContext
			, public GraphicsContext
			, public VulkanGraphicsContext
		{
		public:

			AndroidVulkanGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InAndroidGraphicsContextCreateInformation);
			~AndroidVulkanGraphicsContext();

			virtual void ResetFrameStates() override final;

		};
	}
}

#endif
