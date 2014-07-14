#ifndef _D3D11_RENDERER_HPP_
#define _D3D11_RENDERER_HPP_

#include <d3d11.h>

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Renderer
		{
		private:
			static D3D11Renderer* _inst;
			D3D11Renderer();

		public:
			static D3D11Renderer* get();
			ID3D11Device* getDevice();
		};
	}
}

#endif
