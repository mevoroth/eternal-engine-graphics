#pragma once

#include "Graphics/CommandUtils.hpp"
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
		class AccelerationStructure;
		class ShaderTable;

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

		//////////////////////////////////////////////////////////////////////////

		class CommandList
		{
		public:
			static constexpr uint32_t MaxBufferTransitionsPerSubmit		= 32;
			static constexpr uint32_t MaxTextureTransitionsPerSubmit	= 32;
			static constexpr uint32_t MaxResourceTransitionsPerSubmit	= MaxBufferTransitionsPerSubmit + MaxTextureTransitionsPerSubmit;

			static constexpr uint32_t MaxVertexBuffers					= 16;

			virtual ~CommandList();

			virtual void SetName(_In_ GraphicsContext& InContext, _In_ const std::string& InName);

			virtual void BeginEvent(_In_ GraphicsContext& InContext, _In_ const char* InEventName) = 0;
			virtual void EndEvent(_In_ GraphicsContext& InContext) = 0;

			virtual void Begin(_In_ GraphicsContext& InContext) = 0;
			virtual void End() = 0;

			virtual void BeginRenderPass(const RenderPass& InRenderPass);
			virtual void EndRenderPass();

			virtual void Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) = 0;
			void Transition(_In_ ResourceTransition& InResourceTransition);
			virtual void TransitionUAV(_In_ Resource* InResources[], _In_ uint32_t InResourcesCount) = 0;
			void TransitionUAV(_In_ Resource* InResource);

			virtual void SetViewport(_In_ const Viewport& InViewport) = 0;
			virtual void SetScissorRectangle(_In_ const ScissorRectangle& InScissorRectangle) = 0;
			virtual void SetGraphicsPipeline(_In_ const Pipeline& InPipeline);
			virtual void SetIndexBuffer(_In_ const Resource& InIndexBuffer, _In_ uint32_t InOffset = 0, _In_ const IndexBufferType& InIndexBufferType = IndexBufferType::INDEX_BUFFER_TYPE_16BITS) = 0;
			virtual void SetVertexBuffers(_In_ const Resource* InVertexBuffers[], _In_ uint32_t InBufferCount = 1, _In_ uint32_t InFirstVertexBuffer = 0, _In_ VertexBufferParameters InParameters[] = {}) = 0;
			virtual void SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);
			virtual void DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) = 0;
			virtual void DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstIndex = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) = 0;

			virtual void SetComputePipeline(_In_ const Pipeline& InPipeline) = 0;
			virtual void SetComputeDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);
			virtual void Dispatch(_In_ uint32_t InX = 1, _In_ uint32_t InY = 1, _In_ uint32_t InZ = 1);

			virtual void DispatchMesh(_In_ uint32_t InTaskBatchesCount = 1) = 0;

			virtual void SetRayTracingPipeline(_In_ const Pipeline& InPipeline) = 0;
			void SetRayTracingDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);
			virtual void DispatchRays(_In_ const ShaderTable& InShaderTable, _In_ uint32_t InX = 1, _In_ uint32_t InY = 1) = 0;
			virtual void BuildRaytracingAccelerationStructure(_In_ GraphicsContext& InContext, _In_ AccelerationStructure& InAccelerationStructure) = 0;

			virtual void CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion) = 0;
			virtual void TransferResource(_In_ Resource& InDestinationResource, _In_ Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);

			inline CommandAllocator& GetCommandAllocator() { return _CommandAllocator; }
			inline Device& GetDevice() { return _Device; }

		protected:
			CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			void SetCurrentRootSignature(_In_ const RootSignature* InCurrentSignature) { _CurrentRootSignature = InCurrentSignature; }
			const RootSignature* GetCurrentSignature() const { return _CurrentRootSignature; }

		private:
			void _SetDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);

			Device&					_Device;
			CommandAllocator&		_CommandAllocator;
			const RootSignature*	_CurrentRootSignature	= nullptr;
			CommandListState		_CommandListState		= CommandListState::COMMAND_LIST_STATE_CLOSED;
			std::string				_CommandListName;
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

		class CommandListEventScope
		{
		public:
			CommandListEventScope(_In_ CommandList* InCommandList, _In_ GraphicsContext& InContext, _In_ const char* InEventName);
			~CommandListEventScope();
		private:
			GraphicsContext& _GraphicsContext;
			CommandList* _CommandList = nullptr;
		};
	}
}
