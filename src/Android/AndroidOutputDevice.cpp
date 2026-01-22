#if ETERNAL_PLATFORM_ANDROID

#include "Android/AndroidOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		AndroidOutputDevice::AndroidOutputDevice(_In_ AndroidOutputDeviceCreateInformation& InOutputDeviceCreateInformation)
			: OutputDevice(InOutputDeviceCreateInformation)
		{
		}
	}
}

#endif
