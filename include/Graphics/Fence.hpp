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

			virtual void Wait(_In_ Device& InDevice) = 0;
			virtual void Reset(_In_ Device& InDevice) = 0;
		};
	}
}
