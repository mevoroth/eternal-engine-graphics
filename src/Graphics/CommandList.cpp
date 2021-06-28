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

		ResourceTransitionScope::ResourceTransitionScope(_In_ CommandList& InCommandList, _In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
			: _CommandList(InCommandList)
			, _ResourceTransitionsCount(InResourceTransitionsCount)
		{
			ETERNAL_ASSERT(InResourceTransitionsCount > 0);

			_ReverseTransitions.fill(ResourceTransition());

			for (uint32_t TransitionIndex = 0; TransitionIndex < InResourceTransitionsCount; ++TransitionIndex)
			{
				_ReverseTransitions[TransitionIndex] = InResourceTransitions[TransitionIndex];

				_ReverseTransitions[TransitionIndex].NeedsResolve	= false;
				_ReverseTransitions[TransitionIndex].Before			= InResourceTransitions[TransitionIndex].GetAfter();
				_ReverseTransitions[TransitionIndex].After			= InResourceTransitions[TransitionIndex].GetBefore();
			}

			_CommandList.Transition(InResourceTransitions, InResourceTransitionsCount);
		}

		ResourceTransitionScope::~ResourceTransitionScope()
		{
			_CommandList.Transition(_ReverseTransitions.data(), _ResourceTransitionsCount);
		}
	}
}
