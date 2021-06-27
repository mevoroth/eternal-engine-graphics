#include "Graphics/CommandUtils.h"

#include "Graphics/View.hpp"
#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ResourceTransition::ResourceTransition( _In_ View* InView, _In_ const TransitionState& InAfter, _In_ const CommandType& InBeforeCommandType /* = CommandType::COMMAND_TYPE_GRAPHIC */, _In_ const CommandType& InAfterCommandType /* = CommandType::COMMAND_TYPE_GRAPHIC */ )
			: NeedsResolve(true)
			, ResourceToTransition(InView)
			, After(InAfter)
			, BeforeCommandType(InBeforeCommandType)
			, AfterCommandType(InAfterCommandType)
		{
		}

		const TransitionState& ResourceTransition::GetBefore() const
		{
			return NeedsResolve ? ResourceToTransition->GetResource().GetResourceState() : Before;
		}

		const TransitionState& ResourceTransition::GetAfter() const
		{
			return After;
		}
	}
}
