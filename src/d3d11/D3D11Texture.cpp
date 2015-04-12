#include "d3d11/D3D11Texture.hpp"

using namespace Eternal::Graphics;

static const D3D11_TEXTURE_ADDRESS_MODE ADDRESSMODES[Texture::AddressMode::ADRESSMODE_COUNT] =
{
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP
};

D3D11_TEXTURE_ADDRESS_MODE D3D11Texture::GetD3D11UAddressMode() const
{
	return ADDRESSMODES[GetUAddressMode()];
}

D3D11_TEXTURE_ADDRESS_MODE D3D11Texture::GetD3D11VAddressMode() const
{
	return ADDRESSMODES[GetVAddressMode()];
}
