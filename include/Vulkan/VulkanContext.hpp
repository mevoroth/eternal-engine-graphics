#ifndef _VULKAN_CONTEXT_HPP_
#define _VULKAN_CONTEXT_HPP_

#include "NextGenGraphics/Context.hpp"

struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanContext : public Context
		{
		public:
			VulkanContext(_In_ Device& DeviceObj);

			VkSemaphore_T*	GetFrameSemaphore()	{ return _FrameSemaphore; }

		private:
			VkSemaphore_T*	_FrameSemaphore = nullptr;
		};
	}
}

#endif
