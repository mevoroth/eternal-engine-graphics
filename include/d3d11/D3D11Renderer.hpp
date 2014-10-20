#ifndef _D3D11_RENDERER_HPP_
#define _D3D11_RENDERER_HPP_

#include <DirectXMath.h>
#include <d3d11.h>
#include "Graphics/Renderer.hpp"
#include "D3D11Camera.hpp"

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Renderer : public Renderer
		{
		private:
			struct MatrixBuffer
			{
				XMMATRIX model;
				XMMATRIX view;
				XMMATRIX projection;
			};

			ID3D11Device* _device;
			ID3D11DeviceContext* _deviceContext;
			IDXGISwapChain* _swapChain;
			D3D11Camera* _camera;
			RenderTarget** _renderTargets;
			int _renderTargetsCount;
			Material* _material;
			BlendState* _blendMode;
			VertexBuffer* _vertexBuffer;

			HRESULT _CreateDevice();
			HRESULT _CreateSwapChain();
		public:
			D3D11Renderer(_In_ const RenderMode& mode = HARDWARE, _In_ const AntiAliasing& aa = MSAA_4X);
			ID3D11Device* GetDevice();
			ID3D11DeviceContext* GetDeviceContext();

			virtual void AttachCamera(_In_ Camera* camera);
			virtual void SetVBO(_In_ VertexBuffer* vbo);
			virtual void AttachMaterial(_In_ Material* material);
			//virtual void Draw() { assert(false); };
			virtual void DrawIndexed(_In_ const Vertex vertices[], _In_ int verticesCount, _In_ size_t vertexSize,
				_In_ const uint16_t indices[], _In_ int indicesCount);
			virtual void AttachRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count);
			virtual void ClearRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count);
			virtual void SetBlendMode(_In_ BlendState* blendMode);
			virtual void Flush();
		};
	}
}

#endif
