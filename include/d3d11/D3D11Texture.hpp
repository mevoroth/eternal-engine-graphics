#ifndef _D3D11_TEXTURE_HPP_
#define _D3D11_TEXTURE_HPP_

#include <d3d11.h>
#include "Graphics/Texture.hpp"
#include "d3d11/D3D11Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Texture
			: public Texture
			, public D3D11Resource
		{
		public:
			//virtual ID3D11SamplerState* CreateSamplerState() = 0;
			//virtual ID3D11ShaderResourceView* CreateShaderResourceView() = 0;
			D3D11_TEXTURE_ADDRESS_MODE GetD3D11UAddressMode() const;
			D3D11_TEXTURE_ADDRESS_MODE GetD3D11VAddressMode() const;
		};
	}
}

#endif
