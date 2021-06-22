#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "Graphics/SwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Window;
		class Device;
		class CommandQueue;
		class GraphicsContext;
		class View;

		class VulkanSwapChain : public SwapChain
		{
		public:
			VulkanSwapChain(_In_ GraphicsContext& Context);

			virtual void Acquire(GraphicsContext& Context) override;
			virtual void Present(GraphicsContext& Context) override;

			vk::SwapchainKHR& GetSwapChain() { return _SwapChain; }
			vk::SurfaceKHR& GetSurface() { return _Surface; }
			const vector<View*>& GetBackBufferViews() const { return _BackBufferViews; }
			const vector<vk::Image>& GetBackBufferImages() const { return _BackBufferImages; }

		private:
			vector<View*>				_BackBufferViews;
			vector<vk::Image>			_BackBufferImages;
			vk::SurfaceKHR				_Surface;
			vk::SwapchainKHR			_SwapChain;
			uint32_t					_FrameIndex	= 0u;
		};
	}
}
