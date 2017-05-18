#include "d3d12/D3D12Sampler.hpp"

#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"

using namespace Eternal::Graphics;

D3D12Sampler::D3D12Sampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
	: Sampler(MINLinear, MAGLinear, MIPLinear, U, V, W/*, 0xFFFFFFFF*/)
{
	ETERNAL_ASSERT(!Comparison);

	D3D12_SAMPLER_DESC SamplerDesc;

	SamplerDesc.Filter			= (D3D12_FILTER)((Comparison ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT : 0x0) | (MINLinear ? D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT : 0x0) | (MAGLinear ? D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : 0x0) | (MIPLinear ? D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR : 0x0));
	SamplerDesc.AddressU		= (D3D12_TEXTURE_ADDRESS_MODE)U;
	SamplerDesc.AddressV		= (D3D12_TEXTURE_ADDRESS_MODE)V;
	SamplerDesc.AddressW		= (D3D12_TEXTURE_ADDRESS_MODE)W;
	SamplerDesc.MipLODBias		= 0.f;
	SamplerDesc.MaxAnisotropy	= 1;
	SamplerDesc.ComparisonFunc	= D3D12_COMPARISON_FUNC_NEVER;
	SamplerDesc.BorderColor[0]	= 0.f;
	SamplerDesc.BorderColor[1]	= 0.f;
	SamplerDesc.BorderColor[2]	= 0.f;
	SamplerDesc.BorderColor[3]	= 0.f;
	SamplerDesc.MinLOD			= 0.f;
	SamplerDesc.MaxLOD			= D3D12_FLOAT32_MAX;

	static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateSampler(
		&SamplerDesc,
		static_cast<D3D12DescriptorHeap&>(DescriptorHeapObj).Pop()
	);
}

D3D12Sampler::D3D12Sampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
	: Sampler(false, false, false, U, V, W/*, 0xFFFFFFFF*/)
{
	// TODO: Implement anisotropic
	ETERNAL_ASSERT(false);
}

D3D12Sampler::D3D12Sampler(_In_ Device& DeviceObj, _In_ uint32_t Register, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
	: Sampler(MINLinear, MAGLinear, MIPLinear, U, V, W/*, Register*/)
{
	ETERNAL_ASSERT(!Comparison);
}

void D3D12Sampler::GetD3D12StaticSampler(_Out_ D3D12_STATIC_SAMPLER_DESC& StaticSamplerDesc) const
{
	const bool Comparison = false;

	StaticSamplerDesc.Filter			= (D3D12_FILTER)((Comparison ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT : 0x0) | (GetMINLinear() ? D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT : 0x0) | (GetMAGLinear() ? D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : 0x0) | (GetMIPLinear() ? D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR : 0x0));
	StaticSamplerDesc.AddressU			= (D3D12_TEXTURE_ADDRESS_MODE)GetU();
	StaticSamplerDesc.AddressV			= (D3D12_TEXTURE_ADDRESS_MODE)GetV();
	StaticSamplerDesc.AddressW			= (D3D12_TEXTURE_ADDRESS_MODE)GetW();
	StaticSamplerDesc.MipLODBias		= 0.f;
	StaticSamplerDesc.MaxAnisotropy		= 1;
	StaticSamplerDesc.ComparisonFunc	= D3D12_COMPARISON_FUNC_NEVER;
	StaticSamplerDesc.BorderColor		= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	StaticSamplerDesc.MinLOD			= 0.f;
	StaticSamplerDesc.MaxLOD			= D3D12_FLOAT32_MAX;
	StaticSamplerDesc.ShaderRegister	= _Slot;//GetRegister();
	StaticSamplerDesc.RegisterSpace		= 0;
	StaticSamplerDesc.ShaderVisibility	= D3D12_SHADER_VISIBILITY_ALL;
}
