#ifndef _COMMAND_LIST_HPP_
#define _COMMAND_LIST_HPP_

#include <cstdint>
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;
		class Pipeline;
		class CommandAllocator;
		class RootSignature;
		class Resource;
		class Device;
		class RenderPass;
		class RenderTarget;
		class Resource;
		class DescriptorHeap;
		class View;

		enum TransitionState
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
			TRANSITION_PRESENT					= 0x8000
		};

		struct ResourceTransition
		{
			ResourceTransition(_In_ Resource* ResourceObj, _In_ const TransitionState& Before, _In_ const TransitionState& After)
				: ResourceObj(ResourceObj)
				, Before(Before)
				, After(After)
			{
			}

			ResourceTransition() {}

			Resource* ResourceObj	= nullptr;
			TransitionState Before	= TRANSITION_UNDEFINED;
			TransitionState After	= TRANSITION_UNDEFINED;
		};

		struct Position3D
		{
			int X;
			int Y;
			int Z;
		};

		struct Extent
		{
			int Width;
			int Height;
			int Depth;
		};

		enum CommandListType
		{
			COMMAND_LIST_TYPE_GRAPHIC	= 0,
			COMMAND_LIST_TYPE_COMPUTE,
			COMMAND_LIST_TYPE_COPY
		};

		const uint32_t MAX_BINDABLE_RESOURCES = 128u;

		class CommandList
		{
		public:
			CommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type);
			virtual ~CommandList();

			virtual void SetViewport(_In_ const Viewport& ViewportObj) = 0;
			virtual void SetScissorRectangle(_In_ const Viewport& ViewportObj) = 0;
			
			virtual void BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount) = 0;
			virtual void BindConstantBuffer(_In_ uint32_t Slot, _In_ View& ConstantBuffer) = 0;
			virtual void BindDescriptorTable(_In_ uint32_t Slot, _In_ View& DescriptorTable) = 0;
			virtual void BindBuffer(_In_ uint32_t Slot, _In_ View& Buffer) = 0;
			virtual void BindUAV(_In_ uint32_t Slot, _In_ View& UAV) = 0;
			//virtual void BindConstant(_In_ uint32_t Slot, _In_ )
			virtual void Begin() = 0;
			virtual void Begin(_In_ Pipeline& PipelineObj) = 0;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) = 0;
			virtual void DrawIndexed(_In_ uint32_t IndicesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation) = 0;
			virtual void DrawIndexedInstanced(_In_ uint32_t IndicesCount, _In_ uint32_t InstancesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation, _In_ uint32_t StartInstanceLocation) = 0;
			virtual void End() = 0;
			virtual void BeginRenderPass(_In_ RenderPass& RenderPassObj) = 0;
			virtual void EndRenderPass() = 0;
			virtual void SetIndicesBuffer(_In_ Resource* IndicesBuffer) = 0;
			virtual void SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t VerticesBuffersCount, _In_ Resource* VerticesBuffers[]) = 0;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination) = 0;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination, uint64_t SourceOffset, uint64_t DestinationOffset, uint64_t Size) = 0;
			virtual void CopyTexture(_In_ Resource& Source, _In_ Resource& Destination, _In_ const Position3D& SourcePosition, _In_ const Position3D& DestinationPosition, _In_ const Extent& Size) = 0;
			virtual void CopyBufferToTexture(_In_ Resource& Buffer, _In_ Resource& Texture, uint64_t BufferOffset, uint64_t BufferSize, _In_ const Position3D& TexturePosition, _In_ const Extent& Size) = 0;
			virtual void Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount) = 0;

			CommandAllocator* GetCommandAllocator() { return _CommandAllocator; }

		private:
			CommandAllocator*	_CommandAllocator = nullptr;
		};
	}
}

#endif
