#pragma once

#include "Graphics/CommandUtils.h"

namespace Eternal
{
	namespace Graphics
	{
		enum class CommandType;
		class CommandList;
		class GraphicsContext;

		class CommandQueue
		{
		public:
			CommandQueue(_In_ const CommandType& Type);
			virtual ~CommandQueue() {}
			virtual void SubmitCommandLists(_In_ GraphicsContext& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) = 0;

			inline CommandType GetCommandType() const { return _CommandType; }

		private:
			CommandType _CommandType = CommandType::COMMAND_TYPE_GRAPHIC;
		};
	}
}
