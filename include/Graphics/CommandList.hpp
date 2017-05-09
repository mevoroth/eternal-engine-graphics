#ifndef _COMMAND_LIST_HPP_
#define _COMMAND_LIST_HPP_

#include <cstdint>

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

			Resource* ResourceObj	= nullptr;
			TransitionState Before	= TRANSITION_UNDEFINED;
			TransitionState After	= TRANSITION_UNDEFINED;
		};

		class CommandList
		{
		public:
			virtual void SetViewport(_In_ Viewport& ViewportObj) = 0;
			virtual void SetScissorRectangle(_In_ Viewport& ViewportObj) = 0;
			
			virtual void BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount) = 0;
			virtual void Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline& PipelineObj) = 0;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) = 0;
			virtual void DrawIndexed(_In_ uint32_t IndicesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation) = 0;
			virtual void DrawIndexedInstanced(_In_ uint32_t IndicesCount, _In_ uint32_t InstancesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation, _In_ uint32_t StartInstanceLocation) = 0;
			virtual void End() = 0;
			virtual void BeginRenderPass(_In_ RenderPass& RenderPassObj, RenderTarget& RenderTargetObj, _In_ Viewport& ViewportObj) = 0;
			virtual void EndRenderPass() = 0;
			virtual void SetIndicesBuffer(_In_ Resource* IndicesBuffer) = 0;
			virtual void SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t VerticesBuffersCount, _In_ Resource* VerticesBuffers[]) = 0;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination) = 0;
			virtual void Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount) = 0;
		};
	}
}

#endif
