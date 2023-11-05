#pragma once

#if ETERNAL_ENABLE_VULKAN

#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR (1)
#endif
#include "Vulkan/VulkanHeader.hpp"
#include "Graphics/SwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class OutputDevice;
		class Device;
		class CommandQueue;
		class GraphicsContext;
		class View;

		class VulkanSwapChain final : public SwapChain
		{
		public:
			VulkanSwapChain(_In_ GraphicsContext& Context);
			~VulkanSwapChain();

			virtual void Acquire(GraphicsContext& Context) override final;
			virtual void Present(GraphicsContext& Context) override final;

			const vk::SwapchainKHR& GetSwapChain() const { return _SwapChain; }
			const vk::SurfaceKHR& GetSurface() const { return _Surface; }

		private:
			vk::SurfaceKHR				_Surface;
			vk::SwapchainKHR			_SwapChain;
			uint32_t					_FrameIndex	= 0u;
		};
	}
}

#endif
