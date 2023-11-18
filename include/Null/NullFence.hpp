#pragma once

#include "Graphics/Fence.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullFence final : public Fence
		{
		public:

			virtual void Wait(_In_ Device& InDevice) override final {}
			virtual void Reset(_In_ Device& InDevice) override final {}
		};
	}
}
