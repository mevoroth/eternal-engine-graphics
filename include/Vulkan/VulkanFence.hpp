#pragma once

#include "Graphics/Fence.hpp"
#include <vulkan/vulkan.hpp>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		
		class Device;

		class VulkanFence : public Fence
		{
		public:
			VulkanFence(_In_ Device& DeviceObj);
			~VulkanFence();

			virtual void Wait(_In_ Device& DeviceObj) override;
			virtual void Reset(_In_ Device& DeviceObj) override;

			inline vk::Fence& GetVulkanFence() { return _Fence; }

		private:
			Device& _Device;
			vk::Fence _Fence;
		};
	}
}
