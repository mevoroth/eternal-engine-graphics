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
			static constexpr uint32_t MaxCommandListsPerSubmission = 1024;

			virtual ~CommandQueue() {}
			virtual void SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext = nullptr);

			inline CommandType GetCommandType() const { return _CommandType; }

		protected:
			CommandQueue(_In_ const CommandType& Type);

		private:
			CommandType _CommandType = CommandType::COMMAND_TYPE_GRAPHIC;
		};
	}
}
