#include "d3d12/D3D12Sampler.hpp"

#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"

using namespace Eternal::Graphics;

D3D12Sampler::D3D12Sampler(_In_ D3D12Device& DeviceObj, _In_ D3D12DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
{
	ETERNAL_ASSERT(!Comparison);

	D3D12_SAMPLER_DESC SamplerDesc;

	SamplerDesc.Filter = (D3D12_FILTER)((Comparison ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT : 0x0) | (MINLinear ? D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT : 0x0) | (MAGLinear ? D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : 0x0) | (MIPLinear ? D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR : 0x0));
	SamplerDesc.AddressU = (D3D12_TEXTURE_ADDRESS_MODE)U;
	SamplerDesc.AddressV = (D3D12_TEXTURE_ADDRESS_MODE)V;
	SamplerDesc.AddressW = (D3D12_TEXTURE_ADDRESS_MODE)W;
	SamplerDesc.MipLODBias = 0.f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	SamplerDesc.BorderColor[0] = 0.f;
	SamplerDesc.BorderColor[1] = 0.f;
	SamplerDesc.BorderColor[2] = 0.f;
	SamplerDesc.BorderColor[3] = 0.f;
	SamplerDesc.MinLOD = 0.f;
	SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

	DeviceObj.GetD3D12Device()->CreateSampler(
		&SamplerDesc,
		DescriptorHeapObj.Pop()
	);
}

D3D12Sampler::D3D12Sampler(_In_ D3D12Device& DeviceObj, _In_ D3D12DescriptorHeap& DescriptorHeapObj, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
{
	// TODO: Implement anisotropic
	ETERNAL_ASSERT(false);
}
