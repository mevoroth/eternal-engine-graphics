#ifndef _D3D11_TEXTURE_HPP_
#define _D3D11_TEXTURE_HPP_

#include <d3d11.h>

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Texture
		{
		public:
			virtual ID3D11SamplerState* CreateSamplerState() = 0;
		};
	}
}

#endif
