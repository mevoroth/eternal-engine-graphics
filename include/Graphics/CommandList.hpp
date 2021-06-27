#pragma once

#include "Graphics/CommandUtils.h"

namespace Eternal
{
	namespace Graphics
	{
		class CommandAllocator;
		class RenderPass;

		class CommandList
		{
		public:
			static constexpr uint32_t MaxBufferTransitionsPerSubmit		= 32;
			static constexpr uint32_t MaxTextureTransitionsPerSubmit	= 32;
			static constexpr uint32_t MaxResourceTransitionsPerSubmit	= MaxBufferTransitionsPerSubmit + MaxTextureTransitionsPerSubmit;

			CommandList(_In_ CommandAllocator& InCommandAllocator);
			virtual ~CommandList();

			virtual void Begin() = 0;
			virtual void End() = 0;

			virtual void BeginRenderPass(RenderPass& InRenderPass) = 0;
			virtual void EndRenderPass() = 0;

			virtual void Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) = 0;

			inline CommandAllocator& GetCommandAllocator() { return _CommandAllocator; }

		private:
			CommandAllocator& _CommandAllocator;
		};
	}
}
