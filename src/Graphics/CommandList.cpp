#include "Graphics/CommandList.hpp"
#include "Graphics/DescriptorTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandListState operator|(_In_ const CommandListState& InLeftCommandListState, _In_ const CommandListState& InRightCommandListState)
		{
			return static_cast<CommandListState>(
				static_cast<uint32_t>(InLeftCommandListState) | static_cast<uint32_t>(InRightCommandListState)
			);
		}
		CommandListState operator&(_In_ const CommandListState& InLeftCommandListState, _In_ const CommandListState& InRightCommandListState)
		{
			return static_cast<CommandListState>(
				static_cast<uint32_t>(InLeftCommandListState) & static_cast<uint32_t>(InRightCommandListState)
			);
		}
		CommandListState operator~(_In_ const CommandListState& InCommandListState)
		{
			return static_cast<CommandListState>(
				~static_cast<uint32_t>(InCommandListState)
			);
		}
		CommandListState operator|=(_Inout_ CommandListState& InOutCommandListState, _In_ const CommandListState& InOtherCommandListState)
		{
			InOutCommandListState = InOutCommandListState | InOtherCommandListState;
			return InOutCommandListState;
		}
		CommandListState operator&=(_Inout_ CommandListState& InOutCommandListState, _In_ const CommandListState& InOtherCommandListState)
		{
			InOutCommandListState = InOutCommandListState & InOtherCommandListState;
			return InOutCommandListState;
		}

		CommandList::CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: _Device(InDevice)
			, _CommandAllocator(InCommandAllocator)
		{
		}

		CommandList::~CommandList()
		{
		}

		void CommandList::SetName(_In_ GraphicsContext& InContext, _In_ const std::string& InName)
		{
			_CommandListName = InName;
		}

		void CommandList::BeginRenderPass(const RenderPass& InRenderPass)
		{
			ETERNAL_ASSERT((_CommandListState & CommandListState::COMMAND_LIST_STATE_OPENED) == CommandListState::COMMAND_LIST_STATE_CLOSED);
			_CommandListState |= CommandListState::COMMAND_LIST_STATE_OPENED;
		}

		void CommandList::EndRenderPass()
		{
			ETERNAL_ASSERT((_CommandListState & CommandListState::COMMAND_LIST_STATE_OPENED) == CommandListState::COMMAND_LIST_STATE_OPENED);
			_CommandListState &= ~CommandListState::COMMAND_LIST_STATE_OPENED;
			SetCurrentRootSignature(nullptr);
		}

		void CommandList::SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			_SetDescriptorTable(InContext, InDescriptorTable);
		}

		void CommandList::SetComputeDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			_SetDescriptorTable(InContext, InDescriptorTable);
		}

		void CommandList::_SetDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
#if ETERNAL_DEBUG_MATCHING_ROOT_SIGNATURE
			ETERNAL_ASSERT(GetCurrentSignature() && *GetCurrentSignature() == *InDescriptorTable.GetRootSignature());
#elif ETERNAL_DEBUG
			ETERNAL_ASSERT(GetCurrentSignature() && GetCurrentSignature() == InDescriptorTable.GetRootSignature()); // Faster but limiting possibilities
#endif
		}

		void CommandList::Dispatch(_In_ uint32_t InX /* = 1 */, _In_ uint32_t InY /* = 1 */, _In_ uint32_t InZ /* = 1 */)
		{
			ETERNAL_ASSERT(InX > 0 && InY > 0 && InZ > 0);
		}

		//////////////////////////////////////////////////////////////////////////

		ResourceTransitionScope::ResourceTransitionScope(_In_ CommandList& InCommandList, _In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
			: _CommandList(InCommandList)
			, _ResourceTransitionsCount(InResourceTransitionsCount)
		{
			ETERNAL_ASSERT(InResourceTransitionsCount > 0);

			_ReverseTransitions.fill(ResourceTransition());

			for (uint32_t TransitionIndex = 0; TransitionIndex < InResourceTransitionsCount; ++TransitionIndex)
			{
				_ReverseTransitions[TransitionIndex] = InResourceTransitions[TransitionIndex];

				_ReverseTransitions[TransitionIndex].Flags			&= ~TransitionFlags::TRANSITION_FLAGS_NEEDS_RESOLVE;
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
