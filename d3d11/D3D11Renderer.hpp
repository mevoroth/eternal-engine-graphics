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
		class D3D11Renderer : public Renderer<XMVECTOR, XMMATRIX>
		{
		private:
			static D3D11Renderer* _inst;
			ID3D11Device* _device;
			ID3D11DeviceContext* _deviceContext;

		public:
			D3D11Renderer(_In_ RenderMode mode);
			static D3D11Renderer* get();
			ID3D11Device* getDevice();
		};
	}
}

#endif
