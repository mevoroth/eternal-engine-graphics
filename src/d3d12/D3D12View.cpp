#include "d3d12/D3D12View.hpp"

#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Format.hpp"

using namespace Eternal::Graphics;

static void BuildSrvDescription(_In_ const TextureView& View, _Out_ D3D12_SHADER_RESOURCE_VIEW_DESC& ShaderResourceViewDesc)
{
	switch (View)
	{
	case TEXTURE_VIEW_TYPE_1D:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;

		D3D12_TEX1D_SRV& Tex1DSrv = ShaderResourceViewDesc.Texture1D;
		Tex1DSrv.MostDetailedMip		= 0;
		Tex1DSrv.MipLevels				= (UINT)~0;
		Tex1DSrv.ResourceMinLODClamp	= 0.0f;
	} break;
	case TEXTURE_VIEW_TYPE_1DARRAY:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;

		D3D12_TEX1D_ARRAY_SRV& Tex1ArraySrv = ShaderResourceViewDesc.Texture1DArray;
		Tex1ArraySrv.MostDetailedMip		= 0;
		Tex1ArraySrv.MipLevels				= (UINT)~0;
		Tex1ArraySrv.FirstArraySlice		= 0;
		Tex1ArraySrv.ArraySize				= 1;
		Tex1ArraySrv.ResourceMinLODClamp	= 0.0f;
	} break;
	case TEXTURE_VIEW_TYPE_2D:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

		D3D12_TEX2D_SRV& Tex2DSrv = ShaderResourceViewDesc.Texture2D;
		Tex2DSrv.MostDetailedMip		= 0;
		Tex2DSrv.MipLevels				= (UINT)~0;
		Tex2DSrv.PlaneSlice				= 0;
		Tex2DSrv.ResourceMinLODClamp	= 0.0f;
	} break;
	case TEXTURE_VIEW_TYPE_2DARRAY:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;

		ETERNAL_ASSERT(false); // NOT IMPLEMENTED
	} break;
	case TEXTURE_VIEW_TYPE_3D:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		ETERNAL_ASSERT(false); // NOT IMPLEMENTED
	} break;
	case TEXTURE_VIEW_TYPE_CUBE:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		ETERNAL_ASSERT(false); // NOT IMPLEMENTED
	} break;
	case TEXTURE_VIEW_TYPE_CUBEARRAY:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
		ETERNAL_ASSERT(false); // NOT IMPLEMENTED
	} break;
	default:
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_UNKNOWN;
		ETERNAL_ASSERT(false);
		break;
	}
}

D3D12View::D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
	: _DescriptorHeap(static_cast<D3D12DescriptorHeap&>(DescriptorHeapObj))
{
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format					= D3D12_FORMATS[FormatObj];
	ShaderResourceViewDesc.Shader4ComponentMapping	= 0;
	BuildSrvDescription(ViewType, ShaderResourceViewDesc);

	_CpuDescriptor = _DescriptorHeap.Pop();

	static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateShaderResourceView(
		static_cast<D3D12Resource&>(ResourceObj).GetResource(),
		&ShaderResourceViewDesc,
		_CpuDescriptor
	);
}

D3D12View::D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj)
	: _DescriptorHeap(static_cast<D3D12DescriptorHeap&>(DescriptorHeapObj))
{
	_CpuDescriptor = _DescriptorHeap.Pop();

	static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateRenderTargetView(
		static_cast<D3D12Resource&>(ResourceObj).GetResource(),
		nullptr,
		_CpuDescriptor
	);
}

D3D12View::~D3D12View()
{
	_DescriptorHeap.Push(_CpuDescriptor);
}
