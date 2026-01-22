#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Android/AndroidGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		AndroidGraphicsContext::AndroidGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InGraphicsContextCreateInformation)
			: _AndroidOutputDeviceCreateInformation(
				InGraphicsContextCreateInformation.Settings.Width,
				InGraphicsContextCreateInformation.Settings.Height,
				InGraphicsContextCreateInformation.Settings.IsVSync,
				InGraphicsContextCreateInformation.Settings.IsWindowed
			)
			, _AndroidOutputDevice(_AndroidOutputDeviceCreateInformation)
		{
		}

		void DestroyGraphicsContextCreateInformation(_Inout_ GraphicsContextCreateInformation*& InOutGraphicsContextCreateInformation)
		{
			delete InOutGraphicsContextCreateInformation;
			InOutGraphicsContextCreateInformation = nullptr;
		}
	}
}

#endif
