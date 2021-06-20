#include "d3d12/D3D12View.hpp"

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

		ETERNAL_BREAK(); // NOT IMPLEMENTED
	} break;
	case TEXTURE_VIEW_TYPE_3D:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		ETERNAL_BREAK(); // NOT IMPLEMENTED
	} break;
	case TEXTURE_VIEW_TYPE_CUBE:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		ETERNAL_BREAK(); // NOT IMPLEMENTED
	} break;
	case TEXTURE_VIEW_TYPE_CUBEARRAY:
	{
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
		ETERNAL_BREAK(); // NOT IMPLEMENTED
	} break;
	default:
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_UNKNOWN;
		ETERNAL_BREAK();
		break;
	}
}

D3D12View::D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
	: View(FormatObj)
	, _DescriptorHeap(static_cast<D3D12DescriptorHeap&>(DescriptorHeapObj))
{
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format					= D3D12_FORMATS[FormatObj];
	ShaderResourceViewDesc.Shader4ComponentMapping	= D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0, D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1, D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2, D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3);
	BuildSrvDescription(ViewType, ShaderResourceViewDesc);

	_Handle = _DescriptorHeap.Pop();

	static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateShaderResourceView(
		static_cast<D3D12Resource&>(ResourceObj).GetD3D12Resource(),
		&ShaderResourceViewDesc,
		_Handle.Cpu
	);
}

D3D12View::D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj, _In_ const Format& FormatObj)
	: View(FormatObj)
	, _DescriptorHeap(static_cast<D3D12DescriptorHeap&>(DescriptorHeapObj))
{
	_Handle = _DescriptorHeap.Pop();

	if (FormatObj == FORMAT_D32)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
		DepthStencilViewDesc.Format				= D3D12_FORMATS[FormatObj];
		DepthStencilViewDesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D;
		DepthStencilViewDesc.Flags				= D3D12_DSV_FLAG_NONE;
		DepthStencilViewDesc.Texture2D.MipSlice = 0;

		static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateDepthStencilView(
			static_cast<D3D12Resource&>(ResourceObj).GetD3D12Resource(),
			&DepthStencilViewDesc,
			_Handle.Cpu
		);
	}
	else
	{
		D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
		RenderTargetViewDesc.Format					= D3D12_FORMATS[FormatObj];
		RenderTargetViewDesc.Texture2D.MipSlice		= 0;
		RenderTargetViewDesc.Texture2D.PlaneSlice	= 0; // See D3D12_FEATURE_DATA_FORMAT_INFO value in D3D12Device.cpp

		static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateRenderTargetView(
			static_cast<D3D12Resource&>(ResourceObj).GetD3D12Resource(),
			nullptr,
			_Handle.Cpu
		);
	}
}

D3D12View::~D3D12View()
{
	if (&_DescriptorHeap)
		_DescriptorHeap.Push(_Handle);
}

//RenderTarget& D3D12View::GetAsRenderTarget()
//{
//	ETERNAL_BREAK();
//	return *((RenderTarget*)nullptr);
//}
