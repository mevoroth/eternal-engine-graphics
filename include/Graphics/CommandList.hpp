#pragma once

#include "Graphics/CommandUtils.h"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		class CommandAllocator;
		class RenderPass;
		class Pipeline;
		class GraphicsContext;

		class CommandList
		{
		public:
			static constexpr uint32_t MaxBufferTransitionsPerSubmit		= 32;
			static constexpr uint32_t MaxTextureTransitionsPerSubmit	= 32;
			static constexpr uint32_t MaxResourceTransitionsPerSubmit	= MaxBufferTransitionsPerSubmit + MaxTextureTransitionsPerSubmit;

			CommandList(_In_ CommandAllocator& InCommandAllocator);
			virtual ~CommandList();

			virtual void Begin(_In_ GraphicsContext& InContext) = 0;
			virtual void End() = 0;

			virtual void BeginRenderPass(const RenderPass& InRenderPass) = 0;
			virtual void EndRenderPass() = 0;

			virtual void Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) = 0;

			virtual void SetGraphicsPipeline(_In_ const Pipeline& InPipeline) = 0;
			virtual void DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t FirstInstance = 0) = 0;
			virtual void DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstIndex = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) = 0;

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
