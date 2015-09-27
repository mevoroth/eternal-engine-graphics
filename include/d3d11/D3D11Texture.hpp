#ifndef _D3D11_TEXTURE_HPP_
#define _D3D11_TEXTURE_HPP_

#include "Graphics/Texture.hpp"
#include "d3d11/D3D11Resource.hpp"
#include "d3d11/ID3D11ShaderResource.hpp"

struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Texture
			: public Texture
			, public D3D11Resource
			, public ID3D11ShaderResource
		{
		public:
			D3D11Texture(ID3D11Texture2D* TextureObj);
			virtual ~D3D11Texture();
			inline virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() override
			{
				return _ShaderResourceView;
			}

		protected:
			D3D11Texture();
			ID3D11ShaderResourceView* _ShaderResourceView = nullptr;
		};
	}
}

#endif
