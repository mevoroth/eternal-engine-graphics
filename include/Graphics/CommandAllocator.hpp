#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class CommandAllocator
		{
		public:
			virtual ~CommandAllocator() {}
			virtual void Reset() = 0;
		};
	}
}
