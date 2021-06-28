#pragma once

#include "Graphics/CommandUtils.h"
#include <array>

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

			virtual void BeginRenderPass(const RenderPass& InRenderPass) = 0;
			virtual void EndRenderPass() = 0;

			virtual void Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) = 0;

			inline CommandAllocator& GetCommandAllocator() { return _CommandAllocator; }

		private:
			CommandAllocator& _CommandAllocator;
		};

		class ResourceTransitionScope
		{
		public:
			ResourceTransitionScope(_In_ CommandList& InCommandList, _In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount);
			~ResourceTransitionScope();

		private:
			std::array<ResourceTransition, CommandList::MaxResourceTransitionsPerSubmit>	_ReverseTransitions;
			CommandList&																	_CommandList;
			uint32_t																		_ResourceTransitionsCount = 0;
		};
	}
}
