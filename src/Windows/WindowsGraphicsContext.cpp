#if ETERNAL_PLATFORM_WINDOWS

#include "Windows/WindowsGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		WindowsGraphicsContext::WindowsGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InGraphicsContextCreateInformation)
			: _WindowsOutputDeviceCreateInformation(
				InGraphicsContextCreateInformation.Arguments.hInstance,
				InGraphicsContextCreateInformation.Arguments.nCmdShow,
				InGraphicsContextCreateInformation.Arguments.Name,
				InGraphicsContextCreateInformation.Arguments.ClassName,
				InGraphicsContextCreateInformation.Settings.Width,
				InGraphicsContextCreateInformation.Settings.Height
			)
			, _WindowsOutputDevice(_WindowsOutputDeviceCreateInformation)
		{
			_WindowsOutputDevice.Create(InGraphicsContextCreateInformation.Arguments.WindowEventsHandler);
		}

		void DestroyGraphicsContextCreateInformation(_Inout_ GraphicsContextCreateInformation*& InOutGraphicsContextCreateInformation)
		{
			delete InOutGraphicsContextCreateInformation;
			InOutGraphicsContextCreateInformation = nullptr;
		}
	}
}

#endif
