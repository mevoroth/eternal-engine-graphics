#include "d3d12/D3D12View.hpp"

#include <d3d12.h>
#include "d3d12/D3D12GraphicsContext.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const D3D12_RTV_DIMENSION D3D12_RTV_DIMENSIONS[] = {
			D3D12_RTV_DIMENSION_UNKNOWN,
			D3D12_RTV_DIMENSION_BUFFER,
			D3D12_RTV_DIMENSION_TEXTURE1D,
			D3D12_RTV_DIMENSION_TEXTURE1DARRAY,
			D3D12_RTV_DIMENSION_TEXTURE2D,
			D3D12_RTV_DIMENSION_TEXTURE2DARRAY,
			//D3D12_RTV_DIMENSION_TEXTURE2DMS,
			//D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY,
			D3D12_RTV_DIMENSION_TEXTURE3D
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_RTV_DIMENSIONS) == static_cast<int32_t>(ViewRenderTargetType::VIEW_RENDER_TARGET_COUNT), "Mis-match between rtv dimensions and render target view type count");

		D3D12View::D3D12View(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(InViewCreateInformation.Context.GetDevice()).GetD3D12Device();

			D3D12_RENDER_TARGET_VIEW_DESC D3D12RenderTargetViewDesc;
			D3D12RenderTargetViewDesc.Format		= D3D12_FORMATS[static_cast<int32_t>(InViewCreateInformation.GraphicsFormat)].Format;
			D3D12RenderTargetViewDesc.ViewDimension	= D3D12_RTV_DIMENSIONS[static_cast<int32_t>(InViewCreateInformation.ResourceViewRenderTargetType)];
			
			switch (InViewCreateInformation.ResourceViewRenderTargetType)
			{
			case ViewRenderTargetType::VIEW_RENDER_TARGET_UNKOWN:
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY:
				if (InViewCreateInformation.GraphicsResource.IsMultisample())
				{
					D3D12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION(D3D12RenderTargetViewDesc.ViewDimension + (D3D12_RTV_DIMENSION_TEXTURE2DMS - D3D12_RTV_DIMENSION_TEXTURE2D));
				}
				break;
			}

			switch (InViewCreateInformation.ResourceViewRenderTargetType)
			{
			//case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_UNKNOWN:
			case  ViewRenderTargetType::VIEW_RENDER_TARGET_BUFFER:
				D3D12RenderTargetViewDesc.Buffer.FirstElement							= InViewCreateInformation.MetaData.RenderTargetViewBuffer.FirstElement;
				D3D12RenderTargetViewDesc.Buffer.NumElements							= InViewCreateInformation.MetaData.RenderTargetViewBuffer.NumElements;
				break;

			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D:
				D3D12RenderTargetViewDesc.Texture1D.MipSlice							= InViewCreateInformation.MetaData.RenderTargetViewTexture1D.MipSlice;
				break;

			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY:
				D3D12RenderTargetViewDesc.Texture1DArray.MipSlice						= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.MipSlice;
				D3D12RenderTargetViewDesc.Texture1DArray.FirstArraySlice				= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.FirstArraySlice;
				D3D12RenderTargetViewDesc.Texture1DArray.ArraySize						= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.ArraySize;
				break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D:
				if (InViewCreateInformation.GraphicsResource.IsMultisample())
				{
					D3D12RenderTargetViewDesc.Texture2DMS.UnusedField_NothingToDefine	= 0;
				}
				else
				{
					D3D12RenderTargetViewDesc.Texture2D.MipSlice						= InViewCreateInformation.MetaData.RenderTargetViewTexture2D.MipSlice;
					D3D12RenderTargetViewDesc.Texture2D.PlaneSlice						= InViewCreateInformation.MetaData.RenderTargetViewTexture2D.PlaneSlice;
				}
				break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY:
				if (InViewCreateInformation.GraphicsResource.IsMultisample())
				{
					D3D12RenderTargetViewDesc.Texture2DMSArray.FirstArraySlice			= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.FirstArraySlice;
					D3D12RenderTargetViewDesc.Texture2DMSArray.ArraySize				= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.ArraySize;
				}
				else
				{
					D3D12RenderTargetViewDesc.Texture2DArray.MipSlice					= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.MipSlice;
					D3D12RenderTargetViewDesc.Texture2DArray.FirstArraySlice			= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.FirstArraySlice;
					D3D12RenderTargetViewDesc.Texture2DArray.ArraySize					= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.ArraySize;
					D3D12RenderTargetViewDesc.Texture2DArray.PlaneSlice					= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.PlaneSlice;
				}
				break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_3D:
				D3D12RenderTargetViewDesc.Texture3D.MipSlice							= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.MipSlice;
				D3D12RenderTargetViewDesc.Texture3D.FirstWSlice							= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.FirstWSlice;
				D3D12RenderTargetViewDesc.Texture3D.WSize								= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.WSize;
				break;
			}

			_D3D12Handle.D3D12CPUDescriptorHandle = static_cast<D3D12GraphicsContext&>(InViewCreateInformation.Context).AllocateRenderTargetViewDescriptor(_D3D12Handle.DescriptorHandle);

			InD3DDevice->CreateRenderTargetView(
				static_cast<D3D12Resource&>(InViewCreateInformation.GraphicsResource).GetD3D12Resource(),
				&D3D12RenderTargetViewDesc,
				_D3D12Handle.D3D12CPUDescriptorHandle
			);
		}

		D3D12View::~D3D12View()
		{
			switch (GetViewCreateInformation().ResourceViewType)
			{
			case ViewType::VIEW_RENDER_TARGET:
				static_cast<D3D12GraphicsContext&>(GetViewCreateInformation().Context).ReleaseRenderTargetViewDescriptor(_D3D12Handle.DescriptorHandle);
				break;
			}
		}
	}
}
