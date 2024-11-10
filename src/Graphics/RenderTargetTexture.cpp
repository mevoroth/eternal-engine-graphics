#include "Graphics/RenderTargetTexture.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/View.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/ViewFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr ViewRenderTargetType VIEW_RENDER_TARGET_TYPES[] =
		{
			ViewRenderTargetType::VIEW_RENDER_TARGET_UNKNOWN,
			ViewRenderTargetType::VIEW_RENDER_TARGET_BUFFER,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_3D,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY,
			ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VIEW_RENDER_TARGET_TYPES) == static_cast<uint32_t>(ResourceDimension::RESOURCE_DIMENSION_COUNT), "Mismatch between resource dimension and view render target types");

		static constexpr ViewUnorderedAccessType VIEW_UNORDERED_ACCESS_TYPES[] =
		{
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_UNKNOWN,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_BUFFER,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_1D,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_1D_ARRAY,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_2D,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_2D_ARRAY,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_3D,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_2D_ARRAY,
			ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_TEXTURE_2D_ARRAY
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VIEW_UNORDERED_ACCESS_TYPES) == static_cast<uint32_t>(ResourceDimension::RESOURCE_DIMENSION_COUNT), "Mismatch between resource dimension and view unordered access types");

		static constexpr ViewDepthStencilType VIEW_DEPTH_STENCIL_TYPES[] =
		{
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_1D,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_1D,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_1D,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_1D_ARRAY,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D_ARRAY,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D_ARRAY,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D_ARRAY,
			ViewDepthStencilType::VIEW_DEPTH_STENCIL_TEXTURE_2D_ARRAY
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VIEW_DEPTH_STENCIL_TYPES) == static_cast<uint32_t>(ResourceDimension::RESOURCE_DIMENSION_COUNT), "Mismatch between resource dimension and view depth stencil types");

		ViewRenderTargetType ConvertResourceDimensionToViewRenderTargetType(_In_ const ResourceDimension& InResourceDimension)
		{
			return VIEW_RENDER_TARGET_TYPES[static_cast<uint32_t>(InResourceDimension)];
		}

		ViewUnorderedAccessType ConvertResourceDimensionToViewUnorderedAccessType(_In_ const ResourceDimension& InResourceDimension)
		{
			return VIEW_UNORDERED_ACCESS_TYPES[static_cast<uint32_t>(InResourceDimension)];
		}

		ViewDepthStencilType ConvertResourceDimensionToViewDepthStencilType(_In_ const ResourceDimension& InResourceDimension)
		{
			return VIEW_DEPTH_STENCIL_TYPES[static_cast<uint32_t>(InResourceDimension)];
		}

		RenderTargetTextureFlags operator&(_In_ const RenderTargetTextureFlags& InRenderTargetTextureFlagsLeft, _In_ const RenderTargetTextureFlags& InRenderTargetTextureFlagsRight)
		{
			return static_cast<RenderTargetTextureFlags>(
				static_cast<uint32_t>(InRenderTargetTextureFlagsLeft) & static_cast<uint32_t>(InRenderTargetTextureFlagsRight)
			);
		}

		RenderTargetTexture::RenderTargetTexture(_In_ GraphicsContext& InContext, _In_ const TextureResourceCreateInformation& InTextureResourceCreateInformation, _In_ const RenderTargetTextureFlags& InFlags)
			: Texture(InContext, InTextureResourceCreateInformation)
		{
			ETERNAL_ASSERT(InFlags == RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_ALL
						|| InFlags == RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_GRAPHICS
						|| InFlags == RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_COMPUTE);

			if ((InFlags & RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_RENDER_TARGET_DEPTH_STENCIL_VIEW) == RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_RENDER_TARGET_DEPTH_STENCIL_VIEW)
			{
				if (IsDepthStencilFormat(InTextureResourceCreateInformation.TextureInformation.ResourceFormat))
				{
					ViewMetaData MetaData;
					switch (InTextureResourceCreateInformation.TextureInformation.Dimension)
					{
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
					{
					} break;
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
					{
						MetaData.DepthStencilViewTexture1DArray.ArraySize	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
					} break;
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
					{
						MetaData.DepthStencilViewTexture2DArray.ArraySize	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
					} break;
					default:
						ETERNAL_BREAK();
						break;
					}
					DepthStencilViewCreateInformation TextureDepthStencilViewCreateInformation(
						InContext,
						_Texture,
						MetaData,
						InTextureResourceCreateInformation.TextureInformation.ResourceFormat,
						ConvertResourceDimensionToViewDepthStencilType(InTextureResourceCreateInformation.TextureInformation.Dimension)
					);
					_RenderTargetDepthView = CreateDepthStencilView(TextureDepthStencilViewCreateInformation);
				}
				else
				{
					ViewMetaData MetaData;
					switch (InTextureResourceCreateInformation.TextureInformation.Dimension)
					{
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
					{
					} break;
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
					{
						MetaData.RenderTargetViewTexture1DArray.ArraySize	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
					} break;
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
					{
						MetaData.RenderTargetViewTexture2DArray.ArraySize	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
					} break;
					case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
					{
						MetaData.RenderTargetViewTexture3D.WSize			= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
					} break;
					default:
						ETERNAL_BREAK();
						break;
					}
					RenderTargetViewCreateInformation TextureRenderTargetViewCreateInformation(
						InContext,
						_Texture,
						MetaData,
						InTextureResourceCreateInformation.TextureInformation.ResourceFormat,
						ConvertResourceDimensionToViewRenderTargetType(InTextureResourceCreateInformation.TextureInformation.Dimension)
					);
					_RenderTargetDepthView = CreateRenderTargetView(TextureRenderTargetViewCreateInformation);
				}
			}

			if ((InFlags & RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_UNORDERED_ACCESS_VIEW) == RenderTargetTextureFlags::RENDER_TARGET_TEXTURE_FLAGS_UNORDERED_ACCESS_VIEW)
			{
				ViewMetaData MetaData;
				switch (InTextureResourceCreateInformation.TextureInformation.Dimension)
				{
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
				{
				} break;
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
				{
					MetaData.UnorderedAccessViewTexture1DArray.ArraySize	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
				} break;
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
				{
					MetaData.UnorderedAccessViewTexture2DArray.ArraySize	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
				} break;
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
				{
					MetaData.UnorderedAccessViewTexture3D.WSize				= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
				} break;
				default:
					ETERNAL_BREAK();
					break;
				}
				UnorderedAccessViewCreateInformation TextureUnorderedAccessViewCreateInformation(
					InContext,
					_Texture,
					MetaData,
					ConvertDepthStencilFormatToFormat(InTextureResourceCreateInformation.TextureInformation.ResourceFormat),
					ConvertResourceDimensionToViewUnorderedAccessType(InTextureResourceCreateInformation.TextureInformation.Dimension)
				);
				_UnorderedAccessView = CreateUnorderedAccessView(TextureUnorderedAccessViewCreateInformation);
			}
		}

		RenderTargetTexture::~RenderTargetTexture()
		{
			if (_RenderTargetDepthView)
				DestroyView(_RenderTargetDepthView);
			if (_UnorderedAccessView)
				DestroyView(_UnorderedAccessView);
		}
	}
}
