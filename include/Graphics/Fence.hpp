#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class Fence
		{
		public:
			virtual ~Fence() {}

			virtual void Wait(_In_ Device& DeviceObj) = 0;
			virtual void Reset(_In_ Device& DeviceObj) = 0;
		};
	}
}
