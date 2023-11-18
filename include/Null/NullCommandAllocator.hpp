#pragma once

#include "Graphics/CommandAllocator.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullCommandAllocator final : public CommandAllocator
		{
		public:

			virtual void Reset() override final {}

		};
	}
}
