#pragma once

#if ETERNAL_PLATFORM_ANDROID

#include "OutputDevice/OutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		struct AndroidOutputDeviceCreateInformation : public OutputDeviceCreateInformation
		{
			AndroidOutputDeviceCreateInformation(
				_In_ int InWidth,
				_In_ int InHeight,
				_In_ bool InVSync = false,
				_In_ bool InWindowed = true
			)
				: OutputDeviceCreateInformation(
					InWidth,
					InHeight,
					InVSync,
					InWindowed
				)
			{
			}
		};


		class AndroidOutputDevice : public OutputDevice
		{
		public:

			AndroidOutputDevice(_In_ AndroidOutputDeviceCreateInformation& InOutputDeviceCreateInformation);

		};
	}
}

#endif
