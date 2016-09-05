#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <cstdint>
#include "Macros/Macros.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class RenderTarget;
		class Shader;
		class Constant;
		class Resource;
		class Sampler;
		class Viewport;
		class BlendState;
		class VertexBuffer;
		class IndexBuffer;
		class DepthTest;
		class StencilTest;
		class Clearable;
		class DepthStencil;

		class Context
		{
		public:
			enum ShaderStage
			{
				VERTEX,
				GEOMETRY,
				PIXEL
			};

			enum Topology
			{
				LINELIST,
				TRIANGLELIST,

				TOPOLOGY_COUNT
			};

			Context(_In_ bool IsDeferred)
				: _IsDeferred(IsDeferred)
			{
			}
			virtual ~Context() {}

			bool IsDeferred() const
			{
				return _IsDeferred;
			}

			void Begin()
			{
#ifdef ETERNAL_DEBUG
				ETERNAL_ASSERT(_IsDeferred);
				ETERNAL_ASSERT(!_Enqueueing);
				_Enqueueing = true;
#endif
				BeginCommandList();
			}
			void End()
			{
				ETERNAL_ASSERT(_Enqueueing);
				EndCommandList();
#ifdef ETERNAL_DEBUG
				_Enqueueing = false;
#endif
			}
			virtual void Flush(Context& ContextObj) = 0;
			virtual void SetTopology(_In_ const Topology& TopologyObj) = 0;
			virtual void SetViewport(_In_ Viewport* ViewportObj) = 0;
			virtual void SetBlendMode(_In_ BlendState* BlendStateObj) = 0;
			virtual void DrawIndexed(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer) = 0;
			virtual void DrawDirect(_In_ VertexBuffer* VerticesBuffer) = 0;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) = 0;
			//template<class Vertex> virtual void DrawIndexInstanced()
			virtual void SetRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount) = 0;
			virtual void SetDepthBuffer(_In_ RenderTarget* DepthBuffer) = 0;
			virtual void BindDepthStencilState(_In_ DepthStencil* DepthStencilState) = 0;
			virtual void UnbindDepthStencilState() = 0;
			template<ShaderStage Stage> void BindShader(_In_ Shader* ShaderObj)
			{
				switch (Stage)
				{
				case VERTEX: _BindVertexShader(ShaderObj); break;
				case GEOMETRY: _BindGeometryShader(ShaderObj); break;
				case PIXEL: _BindPixelShader(ShaderObj); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void UnbindShader()
			{
				switch (Stage)
				{
				case VERTEX: _UnbindVertexShader(); break;
				case GEOMETRY: _UnbindGeometryShader(); break;
				case PIXEL: _UnbindPixelShader(); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void BindConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj)
			{
				switch (Stage)
				{
				case VERTEX: _BindVSConstant(Slot, ConstantObj); break;
				case GEOMETRY: _BindGSConstant(Slot, ConstantObj); break;
				case PIXEL: _BindPSConstant(Slot, ConstantObj); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void UnbindConstant(_In_ uint32_t Slot)
			{
				switch (Stage)
				{
				case VERTEX: _UnbindVSConstant(Slot); break;
				case GEOMETRY: _UnbindGSConstant(Slot); break;
				case PIXEL: _UnbindPSConstant(Slot); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void BindBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
			{
				switch (Stage)
				{
				case VERTEX: _BindVSBuffer(Slot, BufferObj); break;
				case GEOMETRY: _BindGSBuffer(Slot, BufferObj); break;
				case PIXEL: _BindPSBuffer(Slot, BufferObj); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void UnbindBuffer(_In_ uint32_t Slot)
			{
				switch (Stage)
				{
				case VERTEX: _UnbindVSBuffer(Slot); break;
				case GEOMETRY: _UnbindGSBuffer(Slot); break;
				case PIXEL: _UnbindPSBuffer(Slot); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void BindUAV()
			{
				ETERNAL_ASSERT(false);
				//switch (Stage)
				//{
				//}
			}
			template<ShaderStage Stage> void BindSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj)
			{
				switch (Stage)
				{
				case VERTEX: _BindVSSampler(Slot, SamplerObj); break;
				case GEOMETRY: _BindGSSampler(Slot, SamplerObj); break;
				case PIXEL: _BindPSSampler(Slot, SamplerObj); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
			template<ShaderStage Stage> void UnbindSampler(_In_ uint32_t Slot)
			{
				switch (Stage)
				{
				case VERTEX: _UnbindVSSampler(Slot); break;
				case GEOMETRY: _UnbindGSSampler(Slot); break;
				case PIXEL: _UnbindPSSampler(Slot); break;
				default: ETERNAL_ASSERT(false); break;
				}
			}
		protected:
			virtual void _BindVertexShader(_In_ Shader* ShaderObj) = 0;
			virtual void _BindGeometryShader(_In_ Shader* ShaderObj) = 0;
			virtual void _BindPixelShader(_In_ Shader* ShaderObj) = 0;
			virtual void _UnbindVertexShader() = 0;
			virtual void _UnbindGeometryShader() = 0;
			virtual void _UnbindPixelShader() = 0;
			virtual void _BindVSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) = 0;
			virtual void _BindGSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) = 0;
			virtual void _BindPSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) = 0;
			virtual void _UnbindVSConstant(_In_ uint32_t Slot) = 0;
			virtual void _UnbindGSConstant(_In_ uint32_t Slot) = 0;
			virtual void _UnbindPSConstant(_In_ uint32_t Slot) = 0;
			virtual void _BindVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) = 0;
			virtual void _BindGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) = 0;
			virtual void _BindPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) = 0;
			virtual void _UnbindVSBuffer(_In_ uint32_t Slot) = 0;
			virtual void _UnbindGSBuffer(_In_ uint32_t Slot) = 0;
			virtual void _UnbindPSBuffer(_In_ uint32_t Slot) = 0;
			virtual void _BindVSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) = 0;
			virtual void _BindGSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) = 0;
			virtual void _BindPSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) = 0;
			virtual void _UnbindVSSampler(_In_ uint32_t Slot) = 0;
			virtual void _UnbindGSSampler(_In_ uint32_t Slot) = 0;
			virtual void _UnbindPSSampler(_In_ uint32_t Slot) = 0;
			virtual void BeginCommandList() = 0;
			virtual void EndCommandList() = 0;

		private:
			bool _IsDeferred = false;
#ifdef ETERNAL_DEBUG
			bool _Enqueueing = false;
#endif
		};
	}
}

#endif
