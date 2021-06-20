#ifndef _VULKAN_CONTEXT_HPP_
#define _VULKAN_CONTEXT_HPP_

#include "NextGenGraphics/Context.hpp"

struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Device;

		class VulkanContext : public Context
		{
		public:
			VulkanContext(_In_ Device& DeviceObj);

			vector<VkSemaphore_T*>&	GetFrameCommandListSemaphores() { return _CommandListSemaphores; }
			VkSemaphore_T*			GetFrameSemaphore()				{ return _FrameSemaphore; }

		private:
			vector<VkSemaphore_T*>	_CommandListSemaphores;
			VkSemaphore_T*			_FrameSemaphore = nullptr;
		};
	}
}

#endif
