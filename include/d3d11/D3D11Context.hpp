#ifndef _D3D11_CONTEXT_HPP_
#define _D3D11_CONTEXT_HPP_

#include "Graphics/Context.hpp"

#include "D3D11Sampler.hpp"
#include "D3D11VertexBuffer.hpp"
#include "D3D11IndexBuffer.hpp"

#include <d3d11.h>

#define D3D11_MAX_VERTICES_BUFFER D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
#define D3D11_MAX_RENDERTARGETS 8

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Context : public Context
		{
		public:
			D3D11Context(ID3D11DeviceContext* D3D11ContextObj);

			virtual void DrawIndexed(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer) override;
			virtual void AttachRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount) override;
			virtual void SetViewport(_In_ Viewport* ViewportObj) override;
			virtual void SetBlendMode(_In_ BlendState* BlendStateObj) override;

			inline ID3D11DeviceContext* GetD3D11Context()
			{
				return _DeviceContext;
			}

		protected:
			virtual void _SetVertexShader(_In_ Shader* ShaderObj) override;
			virtual void _SetGeometryShader(_In_ Shader* ShaderObj) override;
			virtual void _SetPixelShader(_In_ Shader* ShaderObj) override;
			virtual void _SetVSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _SetGSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _SetPSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _SetVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _SetGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _SetPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _SetVSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _SetGSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _SetPSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;

		private:
			Viewport* _Viewport = nullptr;
			BlendState* _BlendState = nullptr;

			ID3D11DeviceContext* _DeviceContext = nullptr;
			
			D3D11VertexBuffer* _VerticesBuffer = nullptr;
			D3D11IndexBuffer* _IndicesBuffer = nullptr;
		};
	}
}

#endif
