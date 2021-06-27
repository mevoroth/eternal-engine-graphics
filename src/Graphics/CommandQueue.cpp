#include "Graphics/CommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandQueue::CommandQueue(_In_ const CommandType& Type)
			: _CommandType(Type)
		{
		}

		void CommandQueue::SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext /* = nullptr */)
		{
			ETERNAL_ASSERT(InCommandListsCount < MaxCommandListsPerSubmission);
			ETERNAL_ASSERT(InCommandListsCount > 0);
		}
	}
}
