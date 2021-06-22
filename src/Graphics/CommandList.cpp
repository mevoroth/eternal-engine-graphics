#include "Graphics/CommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandList::CommandList(_In_ CommandAllocator& InCommandAllocator)
			: _CommandAllocator(InCommandAllocator)
		{
		}

		CommandList::~CommandList()
		{
		}
	}
}
