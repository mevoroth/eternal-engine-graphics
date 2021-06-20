#include "NextGenGraphics/Context.hpp"

#include "NextGenGraphics/DeviceFactory.hpp"
#include "NextGenGraphics/Device.hpp"
#include "NextGenGraphics/Context.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"

#include "Graphics_deprecated/SwapChainFactory.hpp"

using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
		{
			switch (CreateInformation.Settings.Driver)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new GraphicsContext(CreateInformation);
#endif
			case DeviceType::VULKAN:
				return new VulkanGraphicsContext(CreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}

		}

		void DestroyGraphicsContext(GraphicsContext*& Context)
		{
			delete Context;
			Context = nullptr;
		}

		GraphicsContext::GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: _Window(CreateInformation.Arguments.hInstance,
					  CreateInformation.Arguments.nCmdShow,
					  CreateInformation.Arguments.Name,
					  CreateInformation.Arguments.ClassName,
					  CreateInformation.Settings.Width,
					  CreateInformation.Settings.Height)
			, _MainViewportFullScreen(0, 0,
									  CreateInformation.Settings.Width, CreateInformation.Settings.Height)
		{
			_Window.Create(CreateInformation.Arguments.WindowEventsHandler);

			_Device = CreateDevice(*this, CreateInformation.Settings.Driver);
			_SwapChain = CreateSwapChain(*this);
		}

		GraphicsContext::~GraphicsContext()
		{
			delete _SwapChain;
			_SwapChain = nullptr;

			delete _Device;
			_Device = nullptr;
		}
	}
}
