#pragma once

#if ETERNAL_PLATFORM_WINDOWS

#include "Graphics/GraphicsContext.hpp"
#include "Windows/WindowsArguments.hpp"
#include "Windows/WindowsOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct WindowsGraphicsContextCreateInformation : public GraphicsContextCreateInformation
		{
			WindowsGraphicsContextCreateInformation(const RenderSettings& InSettings, const WindowsArguments& InArguments)
				: GraphicsContextCreateInformation(InSettings)
				, Arguments(InArguments)
			{
			}

			WindowsArguments Arguments;
		};

		class WindowsGraphicsContext
		{
		public:

			WindowsGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InGraphicsContextCreateInformation);

		protected:

			WindowsOutputDeviceCreateInformation	_WindowsOutputDeviceCreateInformation;
			WindowsOutputDevice						_WindowsOutputDevice;
		};
	}
}

#endif
