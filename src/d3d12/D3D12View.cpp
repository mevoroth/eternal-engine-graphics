#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12View.hpp"

#include "d3d12/D3D12Library.h"
#include "d3d12/D3D12GraphicsContext.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Format.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr D3D12_RTV_DIMENSION D3D12_RTV_DIMENSIONS[] =
		{
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
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_RTV_DIMENSIONS) == static_cast<int32_t>(ViewRenderTargetType::VIEW_RENDER_TARGET_COUNT), "Mismatch between abstraction and d3d12 rtv dimensions");

		static constexpr D3D12_SRV_DIMENSION D3D12_SRV_DIMENSIONS[] =
		{
			D3D12_SRV_DIMENSION_UNKNOWN,
			D3D12_SRV_DIMENSION_BUFFER,
			D3D12_SRV_DIMENSION_TEXTURE1D,
			D3D12_SRV_DIMENSION_TEXTURE1DARRAY,
			D3D12_SRV_DIMENSION_TEXTURE2D,
			D3D12_SRV_DIMENSION_TEXTURE2DARRAY,
			D3D12_SRV_DIMENSION_TEXTURE3D,
			D3D12_SRV_DIMENSION_TEXTURECUBE,
			D3D12_SRV_DIMENSION_TEXTURECUBEARRAY,
			D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SRV_DIMENSIONS) == static_cast<int32_t>(ViewShaderResourceType::VIEW_SHADER_RESOURCE_COUNT), "Mismatch between abstraction and d3d12 srv dimensions");
		
		static constexpr D3D12_DSV_DIMENSION D3D12_DSV_DIMENSIONS[] =
		{
			D3D12_DSV_DIMENSION_UNKNOWN,
			D3D12_DSV_DIMENSION_TEXTURE1D,
			D3D12_DSV_DIMENSION_TEXTURE1DARRAY,
			D3D12_DSV_DIMENSION_TEXTURE2D,
			D3D12_DSV_DIMENSION_TEXTURE2DARRAY,
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_DSV_DIMENSIONS) == static_cast<int32_t>(ViewDepthStencilType::VIEW_DEPTH_STENCIL_COUNT), "Mismatch between abstraction and d3d12 dsv dimensions");

		static constexpr D3D12_UAV_DIMENSION D3D12_UAV_DIMENSIONS[] =
		{
			D3D12_UAV_DIMENSION_UNKNOWN,
			D3D12_UAV_DIMENSION_BUFFER,
			D3D12_UAV_DIMENSION_TEXTURE1D,
			D3D12_UAV_DIMENSION_TEXTURE1DARRAY,
			D3D12_UAV_DIMENSION_TEXTURE2D,
			D3D12_UAV_DIMENSION_TEXTURE2DARRAY,
			D3D12_UAV_DIMENSION_TEXTURE3D
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_UAV_DIMENSIONS) == static_cast<int32_t>(ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_COUNT), "Mismatch between abstraction and d3d12 uav dimensions");

		static D3D12_RTV_DIMENSION ConvertViewRenderTargetTypeToD3D12RenderTargetViewDimension(_In_ const ViewRenderTargetType& InViewRenderTargetType)
		{
			return D3D12_RTV_DIMENSIONS[static_cast<int32_t>(InViewRenderTargetType)];
		}

		static D3D12_SRV_DIMENSION ConvertViewShaderResourceTypeToD3D12ShaderResourceViewDimension(_In_ const ViewShaderResourceType& InViewShaderResourceType)
		{
			return D3D12_SRV_DIMENSIONS[static_cast<int32_t>(InViewShaderResourceType)];
		}

		static D3D12_DSV_DIMENSION ConvertViewDepthStencilTypeToD3D12DepthStencilViewDimension(_In_ const ViewDepthStencilType& InViewDepthStencilType)
		{
			return D3D12_DSV_DIMENSIONS[static_cast<int32_t>(InViewDepthStencilType)];
		}

		static D3D12_UAV_DIMENSION ConvertViewUnorderedAccessTypeToD3D12UnorderedAccessViewDimension(_In_ const ViewUnorderedAccessType& InViewUnorderedAccessType)
		{
			return D3D12_UAV_DIMENSIONS[static_cast<int32_t>(InViewUnorderedAccessType)];
		}

		D3D12View::D3D12View(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(InViewCreateInformation.Context.GetDevice()).GetD3D12Device();

			D3D12_RENDER_TARGET_VIEW_DESC D3D12RenderTargetViewDesc = {};
			D3D12RenderTargetViewDesc.Format		= ConvertFormatToD3D12Format(InViewCreateInformation.GraphicsFormat).Format;
			D3D12RenderTargetViewDesc.ViewDimension	= ConvertViewRenderTargetTypeToD3D12RenderTargetViewDimension(InViewCreateInformation.ResourceViewRenderTargetType);
			
			switch (InViewCreateInformation.ResourceViewRenderTargetType)
			{
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D:
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY:
				if (GetResource().IsMultisample())
				{
					D3D12RenderTargetViewDesc.ViewDimension = static_cast<D3D12_RTV_DIMENSION>(D3D12RenderTargetViewDesc.ViewDimension + (D3D12_RTV_DIMENSION_TEXTURE2DMS - D3D12_RTV_DIMENSION_TEXTURE2D));
				}
				break;
			}

			const ViewMetaData& InMetaData = InViewCreateInformation.MetaData;

			switch (InViewCreateInformation.ResourceViewRenderTargetType)
			{
			case  ViewRenderTargetType::VIEW_RENDER_TARGET_BUFFER:
			{
				D3D12RenderTargetViewDesc.Buffer.FirstElement							= InMetaData.RenderTargetViewBuffer.FirstElement;
				D3D12RenderTargetViewDesc.Buffer.NumElements							= InMetaData.RenderTargetViewBuffer.NumElements;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D:
			{
				D3D12RenderTargetViewDesc.Texture1D.MipSlice							= InMetaData.RenderTargetViewTexture1D.MipSlice;
			} break;

			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY:
			{
				D3D12RenderTargetViewDesc.Texture1DArray.MipSlice						= InMetaData.RenderTargetViewTexture1DArray.MipSlice;
				D3D12RenderTargetViewDesc.Texture1DArray.FirstArraySlice				= InMetaData.RenderTargetViewTexture1DArray.FirstArraySlice;
				D3D12RenderTargetViewDesc.Texture1DArray.ArraySize						= InMetaData.RenderTargetViewTexture1DArray.ArraySize;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D:
			{
				if (GetResource().IsMultisample())
				{
					D3D12RenderTargetViewDesc.Texture2DMS.UnusedField_NothingToDefine	= 0;
				}
				else
				{
					D3D12RenderTargetViewDesc.Texture2D.MipSlice						= InMetaData.RenderTargetViewTexture2D.MipSlice;
					D3D12RenderTargetViewDesc.Texture2D.PlaneSlice						= InMetaData.RenderTargetViewTexture2D.PlaneSlice;
				}
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY:
			{
				if (GetResource().IsMultisample())
				{
					D3D12RenderTargetViewDesc.Texture2DMSArray.FirstArraySlice			= InMetaData.RenderTargetViewTexture2DArray.FirstArraySlice;
					D3D12RenderTargetViewDesc.Texture2DMSArray.ArraySize				= InMetaData.RenderTargetViewTexture2DArray.ArraySize;
				}
				else
				{
					D3D12RenderTargetViewDesc.Texture2DArray.MipSlice					= InMetaData.RenderTargetViewTexture2DArray.MipSlice;
					D3D12RenderTargetViewDesc.Texture2DArray.FirstArraySlice			= InMetaData.RenderTargetViewTexture2DArray.FirstArraySlice;
					D3D12RenderTargetViewDesc.Texture2DArray.ArraySize					= InMetaData.RenderTargetViewTexture2DArray.ArraySize;
					D3D12RenderTargetViewDesc.Texture2DArray.PlaneSlice					= InMetaData.RenderTargetViewTexture2DArray.PlaneSlice;
				}
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_3D:
			{
				D3D12RenderTargetViewDesc.Texture3D.MipSlice							= InMetaData.RenderTargetViewTexture3D.MipSlice;
				D3D12RenderTargetViewDesc.Texture3D.FirstWSlice							= InMetaData.RenderTargetViewTexture3D.FirstWSlice;
				D3D12RenderTargetViewDesc.Texture3D.WSize								= InMetaData.RenderTargetViewTexture3D.WSize;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_UNKNOWN:
			{
				ETERNAL_BREAK();
			} break;
			}

			_D3D12Handle = static_cast<D3D12GraphicsContext&>(InViewCreateInformation.Context).AllocateRenderTargetViewDescriptor();

			InD3DDevice->CreateRenderTargetView(
				static_cast<D3D12Resource&>(GetResource()).GetD3D12Resource(),
				&D3D12RenderTargetViewDesc,
				_D3D12Handle.D3D12CPUDescriptorHandle
			);
		}

		D3D12View::D3D12View(_In_ const ConstantBufferViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			using namespace Eternal::Math;

			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(InViewCreateInformation.Context.GetDevice()).GetD3D12Device();

			D3D12_CONSTANT_BUFFER_VIEW_DESC D3D12ConstantBufferViewDesc = {};
			D3D12ConstantBufferViewDesc.BufferLocation	= static_cast<D3D12Resource*>(InViewCreateInformation.GraphicsResource)->GetD3D12Resource()->GetGPUVirtualAddress()
														+ InViewCreateInformation.MetaData.ConstantBufferView.BufferElementOffset * InViewCreateInformation.GraphicsResource->GetBufferStride();
			D3D12ConstantBufferViewDesc.SizeInBytes		= Align<UINT>(InViewCreateInformation.MetaData.ConstantBufferView.BufferSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

			_D3D12Handle = static_cast<D3D12GraphicsContext&>(InViewCreateInformation.Context).AllocateConstantBufferViewDescriptor();

			InD3DDevice->CreateConstantBufferView(
				&D3D12ConstantBufferViewDesc,
				_D3D12Handle.D3D12CPUDescriptorHandle
			);
		}

		D3D12View::D3D12View(_In_ const ShaderResourceViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(InViewCreateInformation.Context.GetDevice()).GetD3D12Device();
			
			ID3D12Resource* InD3D12Resource = static_cast<D3D12Resource&>(GetResource()).GetD3D12Resource();

			D3D12_SHADER_RESOURCE_VIEW_DESC D3D12ShaderResourceViewDesc = {};

			D3D12ShaderResourceViewDesc.Format					= ConvertFormatToD3D12Format(InViewCreateInformation.GraphicsFormat).Format;
			D3D12ShaderResourceViewDesc.ViewDimension			= ConvertViewShaderResourceTypeToD3D12ShaderResourceViewDimension(InViewCreateInformation.ResourceViewShaderResourceType);
			D3D12ShaderResourceViewDesc.Shader4ComponentMapping	= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			const ViewMetaData& InMetaData = InViewCreateInformation.MetaData;

			switch (InViewCreateInformation.ResourceViewShaderResourceType)
			{
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_BUFFER:
			{
				D3D12ShaderResourceViewDesc.Buffer.FirstElement							= InMetaData.ShaderResourceViewBuffer.FirstElement;
				D3D12ShaderResourceViewDesc.Buffer.NumElements							= InMetaData.ShaderResourceViewBuffer.NumElements;
				D3D12ShaderResourceViewDesc.Buffer.StructureByteStride					= InMetaData.ShaderResourceViewBuffer.StructureByteStride;
				D3D12ShaderResourceViewDesc.Buffer.Flags								= D3D12_BUFFER_SRV_FLAG_NONE;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_1D:
			{
				D3D12ShaderResourceViewDesc.Texture1D.MostDetailedMip					= InMetaData.ShaderResourceViewTexture1D.MostDetailedMip;
				D3D12ShaderResourceViewDesc.Texture1D.MipLevels							= InMetaData.ShaderResourceViewTexture1D.MipLevels;
				D3D12ShaderResourceViewDesc.Texture1D.ResourceMinLODClamp				= InMetaData.ShaderResourceViewTexture1D.ResourceMinLODClamp;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_1D_ARRAY:
			{
				D3D12ShaderResourceViewDesc.Texture1DArray.MostDetailedMip				= InMetaData.ShaderResourceViewTexture1DArray.MostDetailedMip;
				D3D12ShaderResourceViewDesc.Texture1DArray.MipLevels					= InMetaData.ShaderResourceViewTexture1DArray.MipLevels;
				D3D12ShaderResourceViewDesc.Texture1DArray.FirstArraySlice				= InMetaData.ShaderResourceViewTexture1DArray.FirstArraySlice;
				D3D12ShaderResourceViewDesc.Texture1DArray.ArraySize					= InMetaData.ShaderResourceViewTexture1DArray.ArraySize;
				D3D12ShaderResourceViewDesc.Texture1DArray.ResourceMinLODClamp			= InMetaData.ShaderResourceViewTexture1DArray.ResourceMinLODClamp;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_2D:
			{
				D3D12ShaderResourceViewDesc.Texture2D.MostDetailedMip					= InMetaData.ShaderResourceViewTexture2D.MostDetailedMip;
				D3D12ShaderResourceViewDesc.Texture2D.MipLevels							= InMetaData.ShaderResourceViewTexture2D.MipLevels;
				D3D12ShaderResourceViewDesc.Texture2D.PlaneSlice						= InMetaData.ShaderResourceViewTexture2D.PlaneSlice;
				D3D12ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp				= InMetaData.ShaderResourceViewTexture2D.ResourceMinLODClamp;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_2D_ARRAY:
			{
				D3D12ShaderResourceViewDesc.Texture2DArray.MostDetailedMip				= InMetaData.ShaderResourceViewTexture2DArray.MostDetailedMip;
				D3D12ShaderResourceViewDesc.Texture2DArray.MipLevels					= InMetaData.ShaderResourceViewTexture2DArray.MipLevels;
				D3D12ShaderResourceViewDesc.Texture2DArray.FirstArraySlice				= InMetaData.ShaderResourceViewTexture2DArray.FirstArraySlice;
				D3D12ShaderResourceViewDesc.Texture2DArray.ArraySize					= InMetaData.ShaderResourceViewTexture2DArray.ArraySize;
				D3D12ShaderResourceViewDesc.Texture2DArray.PlaneSlice					= InMetaData.ShaderResourceViewTexture2DArray.PlaneSlice;
				D3D12ShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp			= InMetaData.ShaderResourceViewTexture2DArray.ResourceMinLODClamp;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_3D:
			{
				D3D12ShaderResourceViewDesc.Texture3D.MostDetailedMip					= InMetaData.ShaderResourceViewTexture3D.MostDetailedMip;
				D3D12ShaderResourceViewDesc.Texture3D.MipLevels							= InMetaData.ShaderResourceViewTexture3D.MipLevels;
				D3D12ShaderResourceViewDesc.Texture3D.ResourceMinLODClamp				= InMetaData.ShaderResourceViewTexture3D.ResourceMinLODClamp;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_CUBE:
			{
				D3D12ShaderResourceViewDesc.TextureCube.MostDetailedMip					= InMetaData.ShaderResourceViewTextureCube.MostDetailedMip;
				D3D12ShaderResourceViewDesc.TextureCube.MipLevels						= InMetaData.ShaderResourceViewTextureCube.MipLevels;
				D3D12ShaderResourceViewDesc.TextureCube.ResourceMinLODClamp				= InMetaData.ShaderResourceViewTextureCube.ResourceMinLODClamp;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_CUBE_ARRAY:
				D3D12ShaderResourceViewDesc.TextureCubeArray.MostDetailedMip			= InMetaData.ShaderResourceViewTextureCubeArray.MostDetailedMip;
				D3D12ShaderResourceViewDesc.TextureCubeArray.MipLevels					= InMetaData.ShaderResourceViewTextureCubeArray.MipLevels;
				D3D12ShaderResourceViewDesc.TextureCubeArray.First2DArrayFace			= InMetaData.ShaderResourceViewTextureCubeArray.First2DArrayFace;
				D3D12ShaderResourceViewDesc.TextureCubeArray.NumCubes					= InMetaData.ShaderResourceViewTextureCubeArray.NumCubes;
				D3D12ShaderResourceViewDesc.TextureCubeArray.ResourceMinLODClamp		= InMetaData.ShaderResourceViewTextureCubeArray.ResourceMinLODClamp;
				break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_RAYTRACING_ACCELERATION_STRUCTURE:
				D3D12ShaderResourceViewDesc.RaytracingAccelerationStructure.Location	= InD3D12Resource->GetGPUVirtualAddress();
				break;
			default:
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_UNKNOWN:
			{
				ETERNAL_BREAK();
			} break;
			}

			_D3D12Handle = static_cast<D3D12GraphicsContext&>(InViewCreateInformation.Context).AllocateShaderResourceViewDescriptor();

			InD3DDevice->CreateShaderResourceView(
				InViewCreateInformation.ResourceViewShaderResourceType != ViewShaderResourceType::VIEW_SHADER_RESOURCE_RAYTRACING_ACCELERATION_STRUCTURE ? InD3D12Resource : nullptr,
				&D3D12ShaderResourceViewDesc,
				_D3D12Handle.D3D12CPUDescriptorHandle
			);
		}

		D3D12View::D3D12View(_In_ const UnorderedAccessViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(InViewCreateInformation.Context.GetDevice()).GetD3D12Device();

			D3D12_UNORDERED_ACCESS_VIEW_DESC D3D12UnorderedAccessViewDesc = {};

			D3D12UnorderedAccessViewDesc.Format			= ConvertFormatToD3D12Format(InViewCreateInformation.GraphicsFormat).Format;
			D3D12UnorderedAccessViewDesc.ViewDimension	= ConvertViewUnorderedAccessTypeToD3D12UnorderedAccessViewDimension(InViewCreateInformation.ResourceViewUnorderedAccessType);

			const ViewMetaData& InMetaData = InViewCreateInformation.MetaData;

			switch (InViewCreateInformation.ResourceViewUnorderedAccessType)
			{
			case  ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_BUFFER:
			{
				D3D12UnorderedAccessViewDesc.Buffer.FirstElement			= InMetaData.UnorderedAccessViewBuffer.FirstElement;
				D3D12UnorderedAccessViewDesc.Buffer.NumElements				= InMetaData.UnorderedAccessViewBuffer.NumElements;
				D3D12UnorderedAccessViewDesc.Buffer.StructureByteStride		= InMetaData.UnorderedAccessViewBuffer.StructureByteStride;
			} break;
			case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_1D:
			{
				D3D12UnorderedAccessViewDesc.Texture1D.MipSlice				= InMetaData.UnorderedAccessViewTexture1D.MipSlice;
			} break;

			case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_1D_ARRAY:
			{
				D3D12UnorderedAccessViewDesc.Texture1DArray.MipSlice		= InMetaData.UnorderedAccessViewTexture1DArray.MipSlice;
				D3D12UnorderedAccessViewDesc.Texture1DArray.FirstArraySlice	= InMetaData.UnorderedAccessViewTexture1DArray.FirstArraySlice;
				D3D12UnorderedAccessViewDesc.Texture1DArray.ArraySize		= InMetaData.UnorderedAccessViewTexture1DArray.ArraySize;
			} break;
			case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_2D:
			{
				D3D12UnorderedAccessViewDesc.Texture2D.MipSlice				= InMetaData.UnorderedAccessViewTexture2D.MipSlice;
				D3D12UnorderedAccessViewDesc.Texture2D.PlaneSlice			= InMetaData.UnorderedAccessViewTexture2D.PlaneSlice;
			} break;
			case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_2D_ARRAY:
			{
				D3D12UnorderedAccessViewDesc.Texture2DArray.MipSlice		= InMetaData.UnorderedAccessViewTexture2DArray.MipSlice;
				D3D12UnorderedAccessViewDesc.Texture2DArray.FirstArraySlice	= InMetaData.UnorderedAccessViewTexture2DArray.FirstArraySlice;
				D3D12UnorderedAccessViewDesc.Texture2DArray.ArraySize		= InMetaData.UnorderedAccessViewTexture2DArray.ArraySize;
				D3D12UnorderedAccessViewDesc.Texture2DArray.PlaneSlice		= InMetaData.UnorderedAccessViewTexture2DArray.PlaneSlice;
			} break;
			case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_3D:
			{
				D3D12UnorderedAccessViewDesc.Texture3D.MipSlice				= InMetaData.UnorderedAccessViewTexture3D.MipSlice;
				D3D12UnorderedAccessViewDesc.Texture3D.FirstWSlice			= InMetaData.UnorderedAccessViewTexture3D.FirstWSlice;
				D3D12UnorderedAccessViewDesc.Texture3D.WSize				= InMetaData.UnorderedAccessViewTexture3D.WSize;
			} break;
			case ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_UNKNOWN:
			{
				ETERNAL_BREAK();
			} break;
			}

			_D3D12Handle = static_cast<D3D12GraphicsContext&>(InViewCreateInformation.Context).AllocateUnorderedAccessViewDescriptor();

			InD3DDevice->CreateUnorderedAccessView(
				static_cast<D3D12Resource&>(GetResource()).GetD3D12Resource(),
				nullptr,
				&D3D12UnorderedAccessViewDesc,
				_D3D12Handle.D3D12CPUDescriptorHandle
			);
		}

		D3D12View::D3D12View(_In_ const DepthStencilViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(InViewCreateInformation.Context.GetDevice()).GetD3D12Device();

			D3D12_DEPTH_STENCIL_VIEW_DESC D3D12DepthStencilViewDesc = {};

			D3D12DepthStencilViewDesc.Format		= ConvertFormatToD3D12Format(InViewCreateInformation.GraphicsFormat).Format;
			D3D12DepthStencilViewDesc.ViewDimension	= ConvertViewDepthStencilTypeToD3D12DepthStencilViewDimension(InViewCreateInformation.ResourceViewDepthStencilType);
			D3D12DepthStencilViewDesc.Flags			= D3D12_DSV_FLAG_NONE;

			switch (InViewCreateInformation.ResourceViewDepthStencilType)
			{
			case ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_1D:
			{
				D3D12DepthStencilViewDesc.Texture1D.MipSlice				= InViewCreateInformation.MetaData.DepthStencilViewTexture1D.MipSlice;
			} break;
			case ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_1D_ARRAY:
			{
				D3D12DepthStencilViewDesc.Texture1DArray.MipSlice			= InViewCreateInformation.MetaData.DepthStencilViewTexture1DArray.MipSlice;
				D3D12DepthStencilViewDesc.Texture1DArray.FirstArraySlice	= InViewCreateInformation.MetaData.DepthStencilViewTexture1DArray.FirstArraySlice;
				D3D12DepthStencilViewDesc.Texture1DArray.ArraySize			= InViewCreateInformation.MetaData.DepthStencilViewTexture1DArray.ArraySize;
			} break;
			case ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D:
			{
				D3D12DepthStencilViewDesc.Texture2D.MipSlice				= InViewCreateInformation.MetaData.DepthStencilViewTexture2D.MipSlice;
			} break;
			case ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D_ARRAY:
			{
				D3D12DepthStencilViewDesc.Texture2DArray.MipSlice			= InViewCreateInformation.MetaData.DepthStencilViewTexture2DArray.MipSlice;
				D3D12DepthStencilViewDesc.Texture2DArray.FirstArraySlice	= InViewCreateInformation.MetaData.DepthStencilViewTexture2DArray.FirstArraySlice;
				D3D12DepthStencilViewDesc.Texture2DArray.ArraySize			= InViewCreateInformation.MetaData.DepthStencilViewTexture2DArray.ArraySize;
			} break;
			case ViewDepthStencilType::VIEW_DEPTH_STENCIL_UNKNOWN:
			default:
			{
				ETERNAL_BREAK();
			} break;
			}

			_D3D12Handle = static_cast<D3D12GraphicsContext&>(InViewCreateInformation.Context).AllocateDepthStencilViewDescriptor();

			InD3DDevice->CreateDepthStencilView(
				static_cast<D3D12Resource&>(GetResource()).GetD3D12Resource(),
				&D3D12DepthStencilViewDesc,
				_D3D12Handle.D3D12CPUDescriptorHandle
			);
		}

		D3D12View::~D3D12View()
		{
			D3D12GraphicsContext& D3DGraphicsContext = static_cast<D3D12GraphicsContext&>(GetViewCreateInformation().Context);
			switch (GetViewCreateInformation().ResourceViewType)
			{
			case ViewType::VIEW_RENDER_TARGET:
				D3DGraphicsContext.ReleaseRenderTargetViewDescriptor(_D3D12Handle);
				break;

			case ViewType::VIEW_CONSTANT_BUFFER:
				D3DGraphicsContext.ReleaseConstantBufferViewDescriptor(_D3D12Handle);
				break;

			case ViewType::VIEW_SHADER_RESOURCE:
				D3DGraphicsContext.ReleaseShaderResourceViewDescriptor(_D3D12Handle);
				break;

			case ViewType::VIEW_DEPTH_STENCIL:
				D3DGraphicsContext.ReleaseDepthStencilViewDescriptor(_D3D12Handle);
				break;
			}
		}

		D3D12_GPU_VIRTUAL_ADDRESS D3D12View::GetD3D12OffsettedConstantBuffer() const
		{
			ETERNAL_ASSERT(GetViewCreateInformation().ResourceViewType == ViewType::VIEW_CONSTANT_BUFFER);

			const ViewCreateInformation& ViewInformation = GetViewCreateInformation();
			return static_cast<const D3D12Resource&>(GetResource()).GetD3D12Resource()->GetGPUVirtualAddress()
				+ ViewInformation.MetaData.ConstantBufferView.BufferElementOffset * GetResource().GetBufferStride();
		}

		D3D12_GPU_VIRTUAL_ADDRESS D3D12View::GetD3D12OffsettedBuffer() const
		{
			ETERNAL_ASSERT(GetViewCreateInformation().ResourceViewType == ViewType::VIEW_SHADER_RESOURCE);

			const ViewCreateInformation& ViewInformation = GetViewCreateInformation();
			return static_cast<const D3D12Resource&>(GetResource()).GetD3D12Resource()->GetGPUVirtualAddress()
				+ ViewInformation.MetaData.ShaderResourceViewBuffer.FirstElement * GetResource().GetBufferStride();
		}
	}
}

#endif
