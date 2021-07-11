#pragma once

#include "Graphics/CommandUtils.h"
#include <array>
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		union CopyRegion;

		class Device;
		class CommandAllocator;
		class RenderPass;
		class Pipeline;
		class DescriptorTable;
		class GraphicsContext;
		class RootSignature;
		class Viewport;

		enum class CommandListState
		{
			COMMAND_LIST_STATE_CLOSED = 0x0,
			COMMAND_LIST_STATE_OPENED = 0x1
		};

		CommandListState operator|(_In_ const CommandListState& InLeftCommandListState, _In_ const CommandListState& InRightCommandListState);
		CommandListState operator&(_In_ const CommandListState& InLeftCommandListState, _In_ const CommandListState& InRightCommandListState);
		CommandListState operator~(_In_ const CommandListState& InCommandListState);
		CommandListState operator|=(_Inout_ CommandListState& InOutCommandListState, _In_ const CommandListState& InOtherCommandListState);
		CommandListState operator&=(_Inout_ CommandListState& InOutCommandListState, _In_ const CommandListState& InOtherCommandListState);

		class CommandList
		{
		public:
			static constexpr uint32_t MaxBufferTransitionsPerSubmit		= 32;
			static constexpr uint32_t MaxTextureTransitionsPerSubmit	= 32;
			static constexpr uint32_t MaxResourceTransitionsPerSubmit	= MaxBufferTransitionsPerSubmit + MaxTextureTransitionsPerSubmit;

			static constexpr uint32_t MaxVertexBuffers					= 16;

			virtual ~CommandList();

			virtual void SetName(_In_ const std::string& InName) = 0;

			virtual void Begin(_In_ GraphicsContext& InContext) = 0;
			virtual void End() = 0;

			virtual void BeginRenderPass(const RenderPass& InRenderPass);
			virtual void EndRenderPass();

			virtual void Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) = 0;

			virtual void SetViewport(_In_ const Viewport& InViewport) = 0;
			virtual void SetScissorRectangle(_In_ const ScissorRectangle& InScissorRectangle) = 0;
			virtual void SetGraphicsPipeline(_In_ const Pipeline& InPipeline) = 0;
			virtual void SetIndexBuffer(_In_ const Resource& InIndexBuffer, _In_ uint32_t InOffset = 0, _In_ const IndexBufferType& InIndexBufferType = IndexBufferType::INDEX_BUFFER_TYPE_16BITS) = 0;
			virtual void SetVertexBuffers(_In_ const Resource* InVertexBuffers[], _In_ uint32_t InBufferCount = 1, _In_ uint32_t InFirstVertexBuffer = 0, _In_ VertexBufferParameters InParameters[] = {}) = 0;
			virtual void SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);
			virtual void DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t FirstInstance = 0) = 0;
			virtual void DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstIndex = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) = 0;

			virtual void CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion) = 0;

			inline CommandAllocator& GetCommandAllocator() { return _CommandAllocator; }
			inline Device& GetDevice() { return _Device; }

		protected:
			CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			void SetCurrentRootSignature(_In_ const RootSignature* InCurrentSignature) { _CurrentRootSignature = InCurrentSignature; }
			const RootSignature* GetCurrentSignature() const { return _CurrentRootSignature; }

		private:
			Device&					_Device;
			CommandAllocator&		_CommandAllocator;
			const RootSignature*	_CurrentRootSignature = nullptr;
			CommandListState		_CommandListState;
		};

		class ResourceTransitionScope
		{
		public:
			ResourceTransitionScope(_In_ CommandList& InCommandList, _In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount);
			~ResourceTransitionScope();

		private:
			std::array<ResourceTransition, CommandList::MaxResourceTransitionsPerSubmit>	_ReverseTransitions;
			CommandList&																	_CommandList;
			uint32_t																		_ResourceTransitionsCount = 0;
		};
	}
}
