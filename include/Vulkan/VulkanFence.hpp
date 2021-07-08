#pragma once

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
			VulkanFence(_In_ Device& DeviceObj);
			~VulkanFence();

			virtual void Wait(_In_ Device& DeviceObj) override final;
			virtual void Reset(_In_ Device& DeviceObj) override final;

			inline vk::Fence& GetVulkanFence() { return _Fence; }

		private:
			VulkanDevice&	_Device;
			vk::Fence		_Fence;
		};
	}
}
