#ifndef _VULKAN_STATE_HPP_
#define _VULKAN_STATE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;
		class VulkanDevice;
		class VulkanPipeline;
		class VulkanShader;

		class VulkanState
		{
		public:
			VulkanState(_In_ VulkanDevice& DeviceObj, _In_ VulkanPipeline& Pipeline, _In_ VulkanShader& VS, _In_ VulkanShader& PS, _In_ Viewport& ViewportObj);
		};
	}
}

#endif
