#include "OutputDevice/OutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		OutputDevice::OutputDevice(_In_ const OutputDeviceCreateInformation& InOutputDeviceCreateInformation)
			: _OutputDeviceCreateInformation(InOutputDeviceCreateInformation)
		{
		}
	}
}
