#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <cstdint>

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

		class Context
		{
		public:
			enum ShaderStage
			{
				VERTEX,
				GEOMETRY,
				PIXEL
			};
			virtual void AttachRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount) = 0;
			virtual void SetViewport(_In_ Viewport* ViewportObj) = 0;
			virtual void SetBlendMode(_In_ BlendState* BlendStateObj) = 0;
			virtual void DrawIndexed(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer) = 0;
			//template<class Vertex> virtual void DrawIndexInstanced()
			template<enum Stage> void SetShader(_In_ Shader* ShaderObj)
			{
				switch (Stage)
				{
				case VERTEX: _SetVertexShader(ShaderObj); break;
				case GEOMETRY: _SetGeometryShader(ShaderObj); break;
				case PIXEL: _SetPixelShader(ShaderObj); break;
				}
			}
			template<enum Stage> void SetConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj)
			{
				switch (Stage)
				{
				case VERTEX: _SetVSConstant(Slot, ShaderObj); break;
				case GEOMETRY: _SetGSConstant(Slot, ShaderObj); break;
				case PIXEL: _SetPSConstant(Slot, ShaderObj); break;
				}
			}
			template<enum Stage> void SetBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
			{
				switch (Stage)
				{
				case VERTEX: _SetVSBuffer(Slot, ShaderObj); break;
				case GEOMETRY: _SetGSBuffer(Slot, ShaderObj); break;
				case PIXEL: _SetPSBuffer(Slot, ShaderObj); break;
				}
			}
			template<enum Stage> void SetUAV()
			{
				assert(false);
				//switch (Stage)
				//{
				//}
			}
			template<enum Stage> void SetSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj)
			{
				switch (Stage)
				{
				case VERTEX: _SetVSSampler(Slot, ShaderObj); break;
				case GEOMETRY: _SetGSSampler(Slot, ShaderObj); break;
				case PIXEL: _SetPSSampler(Slot, ShaderObj); break;
				}
			}
		protected:
			virtual void _SetVertexShader(_In_ Shader* ShaderObj) = 0;
			virtual void _SetGeometryShader(_In_ Shader* ShaderObj) = 0;
			virtual void _SetPixelShader(_In_ Shader* ShaderObj) = 0;
			virtual void _SetVSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) = 0;
			virtual void _SetGSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) = 0;
			virtual void _SetPSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) = 0;
			virtual void _SetVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) = 0;
			virtual void _SetGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) = 0;
			virtual void _SetPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) = 0;
			virtual void _SetVSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) = 0;
			virtual void _SetGSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) = 0;
			virtual void _SetPSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) = 0;
		};
	}
}

#endif
