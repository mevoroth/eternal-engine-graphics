#include "d3d11/D3D11Texture.hpp"

#include <d3d11.h>

#include "Macros/Macros.hpp"
#include "Graphics/Format.hpp"
#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

static const D3D11TextureFormatInformation TEXTURE_FORMAT[] = {
	{ DXGI_FORMAT_B8G8R8A8_UNORM, 4 },
	{ DXGI_FORMAT_R8G8B8A8_UNORM, 4 }
};

const D3D11TextureFormatInformation& D3D11Texture::GetD3D11TextureFormatInformation(const Format& FormatObj)
{
	ETERNAL_ASSERT(FormatObj < ETERNAL_ARRAYSIZE(TEXTURE_FORMAT));
	return TEXTURE_FORMAT[FormatObj];
}

D3D11Texture::D3D11Texture(ID3D11Texture2D* TextureObj)
	: Texture(BGRA8888)
	, D3D11Resource((ID3D11Resource*)TextureObj)
{
}

D3D11Texture::D3D11Texture(_In_ const Format& FormatObj, _In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode, _In_ uint32_t Width, _In_ uint32_t Height, _In_ const void* Data)
	: Texture(FormatObj, Width, Height)
{
	ID3D11Texture2D* Texture2D;
	D3D11_TEXTURE2D_DESC Tex2DDesc;

	Tex2DDesc.Width = Width;
	Tex2DDesc.Height = Height;
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.Format = GetD3D11TextureFormatInformation(FormatObj).D3D11Format;
	Tex2DDesc.SampleDesc.Count = 1;
	Tex2DDesc.SampleDesc.Quality = 0;
	Tex2DDesc.Usage = (D3D11_USAGE)UsageObj;
	Tex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.CPUAccessFlags = CPUAccessToD3D11(CPUMode);
	Tex2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA SubResourceData;
	if (Data)
	{
		SubResourceData.pSysMem = Data;
		SubResourceData.SysMemPitch = (UINT)(Width * GetD3D11TextureFormatInformation(FormatObj).Size);
		SubResourceData.SysMemSlicePitch = 0;
	}

	HRESULT hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateTexture2D(&Tex2DDesc, Data ? &SubResourceData : nullptr, &Texture2D);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = TEXTURE_FORMAT[FormatObj].D3D11Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateShaderResourceView(Texture2D, &ShaderResourceViewDesc, &_ShaderResourceView);
	ETERNAL_ASSERT(hr == S_OK);

	_SetD3D11Resource(Texture2D);
	_SetCPUAccess(CPUMode);
}

D3D11Texture::D3D11Texture()
	: Texture(BGRA8888)
	, D3D11Resource()
{
}

D3D11Texture::~D3D11Texture()
{
	_ShaderResourceView->Release();
	_ShaderResourceView = nullptr;
}
