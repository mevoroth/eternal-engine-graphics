#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class View;

		enum class TransitionState
		{
			TRANSITION_UNDEFINED				= 0x0,
			TRANSITION_INDIRECT					= 0x1,
			TRANSITION_INDEX_READ				= 0x2,
			TRANSITION_VERTEX_BUFFER_READ		= 0x4,
			TRANSITION_CONSTANT_BUFFER_READ		= 0x8,
			TRANSITION_NON_PIXEL_SHADER_READ	= 0x10,
			TRANSITION_PIXEL_SHADER_READ		= 0x20,
			TRANSITION_SHADER_WRITE				= 0x40,
			TRANSITION_RENDER_TARGET			= 0x80,
			TRANSITION_DEPTH_STENCIL_READ		= 0x100,
			TRANSITION_DEPTH_STENCIL_WRITE		= 0x200,
			TRANSITION_COPY_READ				= 0x400,
			TRANSITION_COPY_WRITE				= 0x800,
			TRANSITION_RESOLVE_DESTINATION		= 0x1000,
			TRANSITION_RESOLVE_SOURCE			= 0x2000,
			TRANSITION_CPU_READ					= 0x4000,
			TRANSITION_CPU_WRITE				= 0x8000,
			TRANSITION_PRESENT					= 0x10000,
			TRANSITION_PREINITIALIZED			= 0x20000,

			TRANSITION_GENERIC_READ				= (TRANSITION_INDIRECT
												| TRANSITION_INDEX_READ
												| TRANSITION_VERTEX_BUFFER_READ
												| TRANSITION_CONSTANT_BUFFER_READ
												| TRANSITION_NON_PIXEL_SHADER_READ
												| TRANSITION_PIXEL_SHADER_READ
												| TRANSITION_COPY_READ)
		};
		
		enum class CommandType
		{
			COMMAND_TYPE_GRAPHIC = 0,
			COMMAND_TYPE_COMPUTE,
			COMMAND_TYPE_COPY,
			COMMAND_TYPE_COUNT
		};

		inline constexpr TransitionState operator|(_In_ const TransitionState& InLeftTransitionState, _In_ const TransitionState& InRightTransitionState)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InLeftTransitionState) | static_cast<uint32_t>(InRightTransitionState)
			);
		}

		inline TransitionState operator&(_In_ const TransitionState& InLeftTransitionState, _In_ const TransitionState& InRightTransitionState)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InLeftTransitionState) & static_cast<uint32_t>(InRightTransitionState)
			);
		}

		inline TransitionState operator~(_In_ const TransitionState& InTransitionState)
		{
			return static_cast<TransitionState>(
				~static_cast<uint32_t>(InTransitionState)
			);
		}

		inline TransitionState operator<<(_In_ const TransitionState& InTransitionState, _In_ uint32_t BitCount)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InTransitionState) << BitCount
			);
		}

		inline TransitionState operator>>(_In_ const TransitionState& InTransitionState, _In_ uint32_t BitCount)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InTransitionState) >> BitCount
			);
		}

		inline bool OnlyHasFlags(_In_ const TransitionState& InTransitionState, _In_ const TransitionState& InFilterState)
		{
			TransitionState InTransitionStateIncludingInFilterState = (InTransitionState & InFilterState);
			TransitionState InTransitionStateExcludingInFilterState = (InTransitionState & ~InFilterState);

			return InTransitionStateIncludingInFilterState != TransitionState::TRANSITION_UNDEFINED
				|| InTransitionStateExcludingInFilterState == TransitionState::TRANSITION_UNDEFINED;
		}

		struct ResourceTransition
		{
			ResourceTransition(
				_In_ View* InView,
				_In_ const TransitionState& InBefore,
				_In_ const TransitionState& InAfter,
				_In_ const CommandType& InBeforeCommandType = CommandType::COMMAND_TYPE_GRAPHIC,
				_In_ const CommandType& InAfterCommandType = CommandType::COMMAND_TYPE_GRAPHIC
			)
				: ResourceToTransition(InView)
				, Before(InBefore)
				, After(InAfter)
				, BeforeCommandType(InBeforeCommandType)
				, AfterCommandType(InAfterCommandType)
			{
			}

			ResourceTransition(
				_In_ View* InView,
				_In_ const TransitionState& InAfter,
				_In_ const CommandType& InBeforeCommandType = CommandType::COMMAND_TYPE_GRAPHIC,
				_In_ const CommandType& InAfterCommandType = CommandType::COMMAND_TYPE_GRAPHIC
			);

			ResourceTransition() {}

			const TransitionState& GetBefore() const;
			const TransitionState& GetAfter() const;

			bool NeedsResolve					= false;
			View* ResourceToTransition			= nullptr;
			TransitionState Before				= TransitionState::TRANSITION_UNDEFINED;
			TransitionState After				= TransitionState::TRANSITION_UNDEFINED;
			CommandType		BeforeCommandType	= CommandType::COMMAND_TYPE_GRAPHIC;
			CommandType		AfterCommandType	= CommandType::COMMAND_TYPE_GRAPHIC;
		};

		struct Position3D
		{
			int X	= 0;
			int Y	= 0;
			int Z	= 0;
		};

		struct Extent
		{
			int Width	= 0;
			int Height	= 0;
			int Depth	= 0;
		};
	}
}
