#pragma once

#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class ResourceType;
		enum class TransitionState;
		class GraphicsContext;
		class Resource;

		enum class ViewType
		{
			VIEW_UNKNOWN = 0,
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
			VIEW_SHADER_RESOURCE_UNKNOWN = 0, // StructuredBuffer
			VIEW_SHADER_RESOURCE_BUFFER,
			VIEW_SHADER_RESOURCE_TEXTURE_1D,
			VIEW_SHADER_RESOURCE_TEXTURE_1D_ARRAY,
			VIEW_SHADER_RESOURCE_TEXTURE_2D,
			VIEW_SHADER_RESOURCE_TEXTURE_2D_ARRAY,
			VIEW_SHADER_RESOURCE_TEXTURE_3D,
			VIEW_SHADER_RESOURCE_TEXTURE_CUBE,
			VIEW_SHADER_RESOURCE_TEXTURE_CUBE_ARRAY,
			VIEW_SHADER_RESOURCE_COUNT
		};

		enum class ViewUnorderedAccessType
		{
			VIEW_UNORDERED_ACCESS_UNKNOWN = 0, // RWStructuredBuffer
			VIEW_UNORDERED_ACCESS_BUFFER,
			VIEW_UNORDERED_ACCESS_TEXTURE_1D,
			VIEW_UNORDERED_ACCESS_TEXTURE_1D_ARRAY,
			VIEW_UNORDERED_ACCESS_TEXTURE_2D,
			VIEW_UNORDERED_ACCESS_TEXTURE_2D_ARRAY,
			VIEW_UNORDERED_ACCESS_TEXTURE_3D,
			VIEW_UNORDERED_ACCESS_COUNT
		};

		enum class ViewRenderTargetType
		{
			VIEW_RENDER_TARGET_UNKOWN = 0,
			VIEW_RENDER_TARGET_BUFFER,
			VIEW_RENDER_TARGET_TEXTURE_1D,
			VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY,
			VIEW_RENDER_TARGET_TEXTURE_2D,
			VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY,
			VIEW_RENDER_TARGET_TEXTURE_3D,
			VIEW_RENDER_TARGET_COUNT
		};

		union ViewMetaData
		{
			//////////////////////////////////////////////////////////////////////////
			// RTV
			struct
			{
				uint64_t FirstElement			= 0;
				uint32_t NumElements			= 0;
			} RenderTargetViewBuffer;

			struct
			{
				uint32_t MipSlice				= 0;
			} RenderTargetViewTexture1D;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
			} RenderTargetViewTexture1DArray;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t PlaneSlice				= 0;
			} RenderTargetViewTexture2D;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
				uint32_t PlaneSlice				= 0;
			} RenderTargetViewTexture2DArray;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstWSlice			= 0;
				uint32_t WSize					= 0;
			} RenderTargetViewTexture3D;

			//////////////////////////////////////////////////////////////////////////
			// CBV
			struct
			{
				uint32_t BufferOffset			= 0;
				uint32_t BufferSize				= 0;
			} ConstantBufferView;

			//////////////////////////////////////////////////////////////////////////
			// SRV
			struct
			{
				uint64_t FirstElement			= 0;
				uint32_t NumElements			= 0;
				uint32_t StructureByteStride	= 0;
			} ShaderResourceViewBuffer;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTexture1D;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTexture1DArray;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				uint32_t PlaneSlice				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTexture2D;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
				uint32_t PlaneSlice				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTexture2DArray;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTexture3D;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTextureCube;

			struct
			{
				uint32_t MostDetailedMip		= 0;
				uint32_t MipLevels				= 0;
				uint32_t First2DArrayFace		= 0;
				uint32_t NumCubes				= 0;
				float ResourceMinLODClamp		= 0.0f;
			} ShaderResourceViewTextureCubeArray;

			ViewMetaData()
				: Dummy()
			{
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(RenderTargetViewBuffer), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(RenderTargetViewTexture1D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(RenderTargetViewTexture1DArray), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(RenderTargetViewTexture2D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(RenderTargetViewTexture2DArray), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(RenderTargetViewTexture3D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ConstantBufferView), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewBuffer), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTexture1D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTexture1DArray), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTexture2D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTexture2DArray), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTexture3D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTextureCube), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(ShaderResourceViewTextureCubeArray), "Dummy must encapsulates all sub structures");
			}

		private:
			struct
			{
				static constexpr uint32_t Size = 3;
				uint64_t Dummy[Size] = {};
			} Dummy;
		};

		struct ViewCreateInformation
		{
			GraphicsContext&		Context;
			Resource*				GraphicsResource				= nullptr;
			ViewMetaData			MetaData;
			Format					GraphicsFormat					= Format::FORMAT_INVALID;
			ViewType				ResourceViewType				= ViewType::VIEW_UNKNOWN;
			ViewShaderResourceType	ResourceViewShaderResourceType	= ViewShaderResourceType::VIEW_SHADER_RESOURCE_UNKNOWN;
			ViewUnorderedAccessType	ResourceViewUnorderedAccessType	= ViewUnorderedAccessType::VIEW_UNORDERED_ACCESS_UNKNOWN;
			ViewRenderTargetType	ResourceViewRenderTargetType	= ViewRenderTargetType::VIEW_RENDER_TARGET_UNKOWN;

		protected:
			ViewCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData,
				_In_ const Format& InFormat,
				_In_ const ViewType& InViewType
			)
				: Context(InContext)
				, GraphicsResource(InResource)
				, MetaData(InViewMetaData)
				, GraphicsFormat(InFormat)
				, ResourceViewType(InViewType)
			{
			}
		};

		struct RenderTargetViewCreateInformation : public ViewCreateInformation
		{
			RenderTargetViewCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData,
				_In_ const Format& InFormat,
				_In_ const ViewRenderTargetType& InViewRenderTargetType
			)
				: ViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					InFormat,
					ViewType::VIEW_RENDER_TARGET
				)
			{
				ResourceViewRenderTargetType = InViewRenderTargetType;
			}
		};

		struct ConstantBufferViewCreateInformation : public ViewCreateInformation
		{
			ConstantBufferViewCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData,
				_In_ const Format& InFormat = Format::FORMAT_UNKNOWN
			)
				: ViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					InFormat,
					ViewType::VIEW_CONSTANT_BUFFER
				)
			{
			}
		};

		struct ShaderResourceViewCreateInformation : public ViewCreateInformation
		{
			ShaderResourceViewCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData,
				_In_ const Format& InFormat,
				_In_ const ViewShaderResourceType& InViewShaderResourceType
			)
				: ViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					InFormat,
					ViewType::VIEW_SHADER_RESOURCE
				)
			{
				ResourceViewShaderResourceType = InViewShaderResourceType;
			}
		};

		class View
		{
		public:
			virtual ~View() {}

			Resource& GetResource();
			const Resource& GetResource() const;
			Format GetViewFormat() const { return _ViewCreateInformation.GraphicsFormat; }
			const TransitionState& GetResourceTransition() const;
			ResourceType GetResourceType() const;
			const ViewMetaData& GetViewMetaData() const { return _ViewCreateInformation.MetaData; }
			
		protected:
			View(_In_ const ViewCreateInformation& InViewCreateInformation);
			const ViewCreateInformation& GetViewCreateInformation() const { return _ViewCreateInformation; }

		private:
			ViewCreateInformation _ViewCreateInformation;
		};
	}
}
