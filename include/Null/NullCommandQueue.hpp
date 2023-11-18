#pragma once

#include "Graphics/CommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullCommandQueue final : public CommandQueue
		{
		public:

			NullCommandQueue(_In_ const CommandType& InType = CommandType::COMMAND_TYPE_GRAPHICS)
				: CommandQueue(InType)
			{
			}
			virtual void SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext = nullptr) override final {}

		};
	}
}
