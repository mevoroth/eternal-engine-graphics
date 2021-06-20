#include "NextGenGraphics/Context.hpp"

#include "Graphics_deprecated/FenceFactory.hpp"
#include "NextGenGraphics/DeviceFactory.hpp"
#include "NextGenGraphics/Device.hpp"
#include "Graphics_deprecated/SwapChainFactory.hpp"
#include <vulkan/vulkan.hpp>

using namespace Eternal::Graphics;

Context::Context(_In_ Device& DeviceObj)
{
	_FrameFence = CreateFence(DeviceObj);
}

Context::~Context()
{
	delete _FrameFence;
	_FrameFence = nullptr;
}

namespace Eternal
{
	namespace Graphics
	{
		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
		{
			return new GraphicsContext(CreateInformation);
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
