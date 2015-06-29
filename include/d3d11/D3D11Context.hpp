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
			virtual void SetRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount) override;
			virtual void SetViewport(_In_ Viewport* ViewportObj) override;
			virtual void SetBlendMode(_In_ BlendState* BlendStateObj) override;

			inline ID3D11DeviceContext* GetD3D11Context()
			{
				return _DeviceContext;
			}

		protected:
			virtual void _BindVertexShader(_In_ Shader* ShaderObj) override;
			virtual void _BindGeometryShader(_In_ Shader* ShaderObj) override;
			virtual void _BindPixelShader(_In_ Shader* ShaderObj) override;
			virtual void _UnbindVertexShader() override;
			virtual void _UnbindGeometryShader() override;
			virtual void _UnbindPixelShader() override;
			virtual void _BindVSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _BindGSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _BindPSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _UnbindVSConstant(_In_ uint32_t Slot) override;
			virtual void _UnbindGSConstant(_In_ uint32_t Slot) override;
			virtual void _UnbindPSConstant(_In_ uint32_t Slot) override;
			virtual void _BindVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _BindGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _BindPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _UnbindVSBuffer(_In_ uint32_t Slot) override;
			virtual void _UnbindGSBuffer(_In_ uint32_t Slot) override;
			virtual void _UnbindPSBuffer(_In_ uint32_t Slot) override;
			virtual void _BindVSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _BindGSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _BindPSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _UnbindVSSampler(_In_ uint32_t Slot) override;
			virtual void _UnbindGSSampler(_In_ uint32_t Slot) override;
			virtual void _UnbindPSSampler(_In_ uint32_t Slot) override;

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