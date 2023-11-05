#if ETERNAL_PLATFORM_WINDOWS

#include "Windows/WindowsGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		WindowsGraphicsContext::WindowsGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InGraphicsContextCreateInformation)
			: _WindowsOutputDevice(WindowsOutputDeviceCreateInformation(
				InGraphicsContextCreateInformation.Arguments.hInstance,
				InGraphicsContextCreateInformation.Arguments.nCmdShow,
				InGraphicsContextCreateInformation.Arguments.Name,
				InGraphicsContextCreateInformation.Arguments.ClassName,
				InGraphicsContextCreateInformation.Settings.Width,
				InGraphicsContextCreateInformation.Settings.Height
			))
			, GraphicsContext(InGraphicsContextCreateInformation, _WindowsOutputDevice)
		{
			_WindowsOutputDevice.Create(InGraphicsContextCreateInformation.Arguments.WindowEventsHandler);

		}
	}
}

#endif
