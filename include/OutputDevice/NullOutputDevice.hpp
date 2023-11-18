#pragma once

#include "OutputDevice/OutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct NullOutputDeviceCreateInformation : public OutputDeviceCreateInformation
		{
			NullOutputDeviceCreateInformation()
				: OutputDeviceCreateInformation(1, 1)
			{
			}
		};

		class NullOutputDevice : public OutputDevice
		{
		public:

			NullOutputDevice()
				: OutputDevice(NullOutputDeviceCreateInformation())
			{
			}

		};
	}
}
