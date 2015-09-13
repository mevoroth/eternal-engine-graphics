#include "d3d11/D3D11Texture.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

D3D11Texture::D3D11Texture(ID3D11Texture2D* TextureObj)
	: D3D11Resource((ID3D11Resource*)TextureObj)
{
}

D3D11Texture::D3D11Texture()
	: D3D11Resource()
{
}

ID3D11ShaderResourceView* D3D11Texture::GetD3D11ShaderResourceView()
{
	ETERNAL_ASSERT(false);
	return nullptr;
}
