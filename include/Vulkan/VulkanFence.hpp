#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Graphics/Fence.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		
		class VulkanDevice;

		class VulkanFence final : public Fence
		{
		public:
			VulkanFence(_In_ Device& InDevice);
			~VulkanFence();

			virtual void Wait(_In_ Device& InDevice) override final;
			virtual void Reset(_In_ Device& InDevice) override final;

			inline vk::Fence& GetVulkanFence() { return _Fence; }

		private:
			VulkanDevice&	_Device;
			vk::Fence		_Fence;
		};
	}
}

#endif
