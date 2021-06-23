#pragma once

#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Resource;

		enum class ViewType
		{
			VIEW_UNKNOWN,
			VIEW_INDEX_BUFFER,
			VIEW_VERTEX_BUFFER,
			VIEW_CONSTANT_BUFFER,
			VIEW_SHADER_RESOURCE,
			VIEW_UNORDERED_ACCESS,
			VIEW_RENDER_TARGET,
			VIEW_DEPTH_STENCIL
		};

		enum class ViewShaderResourceType
		{
			VIEW_SHADER_RESOURCE_UNKNOWN, // StructuredBuffer
			VIEW_SHADER_RESOURCE_BUFFER,
			VIEW_SHADER_RESOURCE_TEXTURE_1D,
			VIEW_SHADER_RESOURCE_TEXTURE_1D_ARRAY,
			VIEW_SHADER_RESOURCE_TEXTURE_2D,
			VIEW_SHADER_RESOURCE_TEXTURE_2D_ARRAY,
			VIEW_SHADER_RESOURCE_TEXTURE_3D,
			VIEW_SHADER_RESOURCE_TEXTURE_CUBE,
			VIEW_SHADER_RESOURCE_TEXTURE_CUBE_ARRAY
		};

		enum class ViewUnorderedAccessType
		{
			VIEW_UNORDERED_ACCESS_UNKNOWN, // RWStructuredBuffer
			VIEW_UNORDERED_ACCESS_BUFFER,
			VIEW_UNORDERED_ACCESS_TEXTURE_1D,
			VIEW_UNORDERED_ACCESS_TEXTURE_1D_ARRAY,
			VIEW_UNORDERED_ACCESS_TEXTURE_2D,
			VIEW_UNORDERED_ACCESS_TEXTURE_2D_ARRAY,
			VIEW_UNORDERED_ACCESS_TEXTURE_3D
		};

		enum class ViewRenderTargetType
		{
			VIEW_RENDER_TARGET_UNKOWN,
			VIEW_RENDER_TARGET_BUFFER,
			VIEW_RENDER_TARGET_TEXTURE_1D,
			VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY,
			VIEW_RENDET_TARGET_TEXTURE_2D,
			VIEW_RENDET_TARGET_TEXTURE_2D_ARRAY,
			VIEW_RENDET_TARGET_TEXTURE_3D
		};

		union ViewMetaData
		{
			// RTV
			struct
			{
				uint64_t FirstElement		= 0;
				uint32_t NumElements		= 0;
			} RenderTargetViewBuffer;

			struct
			{
				uint32_t MipSlice			= 0;
			} RenderTargetViewTexture1D;

			struct
			{
				uint32_t MipSlice			= 0;
				uint32_t FirstArraySlice	= 0;
				uint32_t ArraySize			= 0;
			} RenderTargetViewTexture1DArray;

			struct
			{
				uint32_t MipSlice			= 0;
				uint32_t PlaneSlice			= 0;
			} RenderTargetViewTexture2D;

			struct
			{
				uint32_t MipSlice			= 0;
				uint32_t FirstArraySlice	= 0;
				uint32_t ArraySize			= 0;
				uint32_t PlaneSlice			= 0;
			} RenderTargetViewTexture2DArray;

			struct
			{
				uint32_t MipSlice			= 0;
				uint32_t FirstWSlice		= 0;
				uint32_t WSize				= 0;
			} RenderTargetViewTexture3D;
		};

		struct ViewCreateInformation
		{
			ViewCreateInformation(_In_ GraphicsContext& InContext, _In_ Resource& InResource, _In_ const ViewMetaData& InViewMetaData, _In_ const Format& InFormat, _In_ const ViewType& InViewType)
				: Context(InContext)
				, GraphicsResource(InResource)
				, MetaData(InViewMetaData)
				, GraphicsFormat(InFormat)
				, ResourceViewType(InViewType)
			{
			}

			GraphicsContext&		Context;
			Resource&				GraphicsResource;
			ViewMetaData			MetaData;
			Format					GraphicsFormat					= Format::FORMAT_INVALID;
			ViewType				ResourceViewType				= ViewType::VIEW_UNKNOWN;
			ViewShaderResourceType	ResourceViewShaderResourceType	= ViewShaderResourceType::VIEW_SHADER_RESOURCE_UNKNOWN;
			ViewUnorderedAccessType	ResourceViewUnorderedAccessType	= ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_UNKNOWN;
			ViewRenderTargetType	ResourceViewRenderTargetType	= ViewRenderTargetType::VIEW_RENDER_TARGET_UNKOWN;
		};

		struct RenderTargetViewCreateInformation : public ViewCreateInformation
		{
			RenderTargetViewCreateInformation(_In_ GraphicsContext& InContext, _In_ Resource& InResource, _In_ const ViewMetaData& InViewMetaData, _In_ const Format& InFormat, _In_ const ViewRenderTargetType& InViewRenderTargetType)
				: ViewCreateInformation(InContext, InResource, InViewMetaData, InFormat, ViewType::VIEW_RENDER_TARGET)
			{
				ResourceViewRenderTargetType = InViewRenderTargetType;
			}
		};

		class View
		{
		public:
			View(_In_ const ViewCreateInformation& InViewCreateInformation);
			virtual ~View() {}

		private:
			ViewCreateInformation _ViewCreateInformation;
		};
	}
}
