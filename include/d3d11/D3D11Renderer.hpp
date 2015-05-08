#ifndef _D3D11_RENDERER_HPP_
#define _D3D11_RENDERER_HPP_

#include <DirectXMath.h>
#include <d3d11.h>
#include "Graphics/Renderer.hpp"

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		class Material;
		class Camera;
		class D3D11VertexShader;
		class D3D11PixelShader;
		class D3D11InputLayout;

		class D3D11Renderer : public Renderer
		{
		private:
			struct MatrixBuffer
			{
				Matrix4x4 model;
				Matrix4x4 view;
				Matrix4x4 projection;
			};

			ID3D11Device* _Device;
			ID3D11DeviceContext* _DeviceContext;
			IDXGISwapChain* _SwapChain;
			Camera* _Camera;
			RenderTarget** _RenderTargets = nullptr;
			int _RenderTargetsCount = 0;
			Material* _Material = nullptr;
			BlendState* _BlendMode = nullptr;
			VertexBuffer* _vertexBuffer;

#pragma region Deferred
			Material* _DeferredMaterial = nullptr;
			D3D11InputLayout* _DeferredInputLayout = nullptr;
			D3D11VertexShader* _DeferredVS = nullptr;
			D3D11PixelShader* _DeferredPS = nullptr;
#pragma endregion Deferred
			Camera* _PostProcessCam = nullptr;

			HRESULT _CreateDevice();
			HRESULT _CreateSwapChain();
			void _Settings();
		protected:
			virtual inline Matrix4x4 _GetMatrix() const override
			{
				XMMATRIX TempMatrix = XMMatrixTranspose(XMLoadFloat4x4(&Renderer::_GetMatrix()));
				Matrix4x4 ReturnMatrix;
				XMStoreFloat4x4(&ReturnMatrix, TempMatrix);
				return ReturnMatrix;
			}
		public:
			D3D11Renderer(_In_ const RenderMode& mode = HARDWARE, _In_ const AntiAliasing& aa = MSAA_4X);
			ID3D11Device* GetDevice();
			ID3D11DeviceContext* GetDeviceContext();

			virtual void AttachCamera(_In_ Camera* camera) override;
			virtual void SetVBO(_In_ VertexBuffer* vbo) override;
			virtual void AttachMaterial(_In_ Material* material) override;
			//virtual void Draw() { assert(false); };
			virtual void DrawIndexed(_In_ const Vertex vertices[], _In_ int verticesCount, _In_ size_t vertexSize,
				_In_ const uint16_t indices[], _In_ int indicesCount) override;
			virtual void AttachRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count) override;
			virtual void ClearRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count) override;
			virtual void UnbindRenderTargets() override;
			virtual void SetBlendMode(_In_ BlendState* blendMode) override;
			virtual void Flush();
			virtual void BeginDeferred();
			virtual void DrawDeferred(_In_ const Vertex vertices[], _In_ int verticesCount, _In_ size_t vertexSize,
				_In_ const uint16_t indices[], _In_ int indicesCount);
			virtual void EndDeferred();
		};
	}
}

#endif
