#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Resource;

		enum class TransitionState
		{
			TRANSITION_UNDEFINED				= 0x0,
			TRANSITION_INDIRECT					= 0x1,
			TRANSITION_INDEX_READ				= 0x2,
			TRANSITION_VERTEX_ATTRIBUTE_READ	= 0x4,
			TRANSITION_CONSTANT_READ			= 0x8,
			TRANSITION_RENDER_TARGET_READ		= 0x10,
			TRANSITION_SHADER_READ				= 0x20,
			TRANSITION_SHADER_WRITE				= 0x40,
			TRANSITION_BLEND_READ				= 0x80,
			TRANSITION_RENDER_TARGET_WRITE		= 0x100,
			TRANSITION_DEPTH_STENCIL_READ		= 0x200,
			TRANSITION_DEPTH_STENCIL_WRITE		= 0x400,
			TRANSITION_COPY_READ				= 0x800,
			TRANSITION_COPY_WRITE				= 0x1000,
			TRANSITION_CPU_READ					= 0x2000,
			TRANSITION_CPU_WRITE				= 0x4000,
			TRANSITION_PRESENT					= 0x8000,
			TRANSITION_PREINITIALIZED			= 0x10000
		};

		inline TransitionState operator|(TransitionState LeftTransitionState, TransitionState RightTransitionState)
		{
			return TransitionState(
				static_cast<uint32_t>(LeftTransitionState) | static_cast<uint32_t>(RightTransitionState)
			);
		}

		inline TransitionState operator&(TransitionState LeftTransitionState, TransitionState RightTransitionState)
		{
			return TransitionState(
				static_cast<uint32_t>(LeftTransitionState) & static_cast<uint32_t>(RightTransitionState)
			);
		}

		struct ResourceTransition
		{
			ResourceTransition(_In_ Resource* InResource, _In_ const TransitionState& Before, _In_ const TransitionState& After)
				: ResourceToTransition(InResource)
				, Before(Before)
				, After(After)
			{
			}

			ResourceTransition() {}

			Resource* ResourceToTransition	= nullptr;
			TransitionState Before			= TransitionState::TRANSITION_UNDEFINED;
			TransitionState After			= TransitionState::TRANSITION_UNDEFINED;
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

		enum class CommandType
		{
			COMMAND_TYPE_GRAPHIC = 0,
			COMMAND_TYPE_COMPUTE,
			COMMAND_TYPE_COPY,
			COMMAND_TYPE_COUNT
		};
	}
}
