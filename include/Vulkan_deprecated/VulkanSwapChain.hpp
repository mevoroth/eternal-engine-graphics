#ifndef _VULKAN_SWAP_CHAIN_HPP_
#define _VULKAN_SWAP_CHAIN_HPP_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "Graphics_deprecated/SwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Window;
		class Device;
		class CommandQueue;
		class Resource;
		class View;
		class VulkanView;
		class VulkanRenderPass;
		class VulkanFence;
		class VulkanResource;

		class VulkanSwapChain : public SwapChain
		{
		public:
			VulkanSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj);

			virtual void Present() override;

			//virtual void AcquireFrame(_In_ Device& DeviceObj, _Inout_ Context& GfxContext) override;
			//virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _Inout_ Context& GfxContext) override;

			//virtual Resource& GetBackBuffer(_In_ uint32_t BackBufferIndex) override;
			//virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) override;
			virtual uint32_t GetBackBuffersFrameCount() const override;
			virtual RenderPass& GetMainRenderPass() override;
			vk::SwapchainKHR& GetSwapChain() { return _SwapChain; }
			const vector<View*>& GetBackBufferViews() const { return _BackBufferViews; }
			const vector<vk::Image>& GetBackBufferImages() const { return _BackBufferImages; }

			//VkSemaphore_T*& GetAcquireSemaphore(_In_ uint32_t ResourceIndex);

		//private:
			vector<VulkanRenderPass*>	_RenderPasses;
			vector<VulkanResource*>		_BackBuffers;
			vector<View*>				_BackBufferViews;
			//vector<VkSemaphore_T*>		_AcquireSemaphores;
			vector<vk::Image>			_BackBufferImages;
			vk::SurfaceKHR				_Surface;
			vk::SwapchainKHR			_SwapChain;
			uint32_t					_FrameIndex	= 0u;
		};
	}
}

#endif
