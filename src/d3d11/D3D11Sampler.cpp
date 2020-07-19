#include "d3d11/D3D11Sampler.hpp"

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11Sampler::D3D11Sampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
{
	// TODO: Comparison support
	ETERNAL_ASSERT(!Comparison);

	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.Filter = (D3D11_FILTER)((Comparison ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : 0x0) | (MINLinear ? D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT : 0x0) | (MAGLinear ? D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : 0x0) | (MIPLinear ? D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR : 0x0));
	SamplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)U;
	SamplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)V;
	SamplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)W;
	SamplerDesc.MipLODBias = 0.f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.BorderColor[0] = 0.f;
	SamplerDesc.BorderColor[1] = 0.f;
	SamplerDesc.BorderColor[2] = 0.f;
	SamplerDesc.BorderColor[3] = 0.f;
	SamplerDesc.MinLOD = 0.f;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateSamplerState(&SamplerDesc, &_SamplerState);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D11Sampler::D3D11Sampler(_In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
{
	// TODO: Implement anisotropic
	ETERNAL_ASSERT(false);
}

D3D11Sampler::~D3D11Sampler()
{
	_SamplerState->Release();
	_SamplerState = nullptr;
}

ID3D11SamplerState* D3D11Sampler::GetD3D11SamplerState()
{
	return _SamplerState;
}
