#pragma once

#include "Graphics/CommandUtils.h"

namespace Eternal
{
	namespace Graphics
	{
		class CommandAllocator;

		class CommandList
		{
		public:
			CommandList(_In_ CommandAllocator& InCommandAllocator);
			virtual ~CommandList();

			virtual void Begin() = 0;
			virtual void End() = 0;

			inline CommandAllocator& GetCommandAllocator() { return _CommandAllocator; }

		private:
			CommandAllocator& _CommandAllocator;
		};
	}
}
