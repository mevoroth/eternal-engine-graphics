#pragma once

#include "Graphics/Format.hpp"
#include <unordered_set>

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
			VIEW_SHADER_RESOURCE_RAYTRACING_ACCELERATION_STRUCTURE,
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
			VIEW_RENDER_TARGET_UNKNOWN = 0,
			VIEW_RENDER_TARGET_BUFFER,
			VIEW_RENDER_TARGET_TEXTURE_1D,
			VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY,
			VIEW_RENDER_TARGET_TEXTURE_2D,
			VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY,
			VIEW_RENDER_TARGET_TEXTURE_3D,
			VIEW_RENDER_TARGET_COUNT
		};

		enum class ViewDepthStencilType
		{
			VIEW_DEPTH_STENCIL_UNKNOWN			= 0,
			VIEW_DEPTH_STENCIL_TEXTURE_1D,
			VIEW_DEPTH_STENCIL_TEXTURE_1D_ARRAY,
			VIEW_DEPTH_STENCIL_TEXTURE_2D,
			VIEW_DEPTH_STENCIL_TEXTURE_2D_ARRAY,
			VIEW_DEPTH_STENCIL_COUNT
		};

		enum class ViewPlane
		{
			VIEW_PLANE_0		= 0,
			VIEW_PLANE_1,
			VIEW_PLANE_DEPTH	= VIEW_PLANE_0,
			VIEW_PLANE_STENCIL	= VIEW_PLANE_1,
			VIEW_PLANE_DEFAULT	= VIEW_PLANE_0
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
				uint32_t BufferElementOffset	= 0;
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

			//////////////////////////////////////////////////////////////////////////
			// UAV
			struct
			{
				uint32_t FirstElement			= 0;
				uint32_t NumElements			= 0;
				uint32_t StructureByteStride	= 0;
			} UnorderedAccessViewBuffer;

			struct
			{
				uint32_t MipSlice				= 0;
			} UnorderedAccessViewTexture1D;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
			} UnorderedAccessViewTexture1DArray;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t PlaneSlice				= 0;
			} UnorderedAccessViewTexture2D;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
				uint32_t PlaneSlice				= 0;
			} UnorderedAccessViewTexture2DArray;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstWSlice			= 0;
				uint32_t WSize					= 0;
			} UnorderedAccessViewTexture3D;

			//////////////////////////////////////////////////////////////////////////
			// DSV
			struct
			{
				uint32_t MipSlice				= 0;
			} DepthStencilViewTexture1D;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
			} DepthStencilViewTexture1DArray;

			struct
			{
				uint32_t MipSlice				= 0;
			} DepthStencilViewTexture2D;

			struct
			{
				uint32_t MipSlice				= 0;
				uint32_t FirstArraySlice		= 0;
				uint32_t ArraySize				= 0;
			} DepthStencilViewTexture2DArray;

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
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(UnorderedAccessViewBuffer), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(UnorderedAccessViewTexture1D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(UnorderedAccessViewTexture1DArray), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(UnorderedAccessViewTexture2D), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(UnorderedAccessViewTexture2DArray), "Dummy must encapsulates all sub structures");
				ETERNAL_STATIC_ASSERT(sizeof(Dummy) >= sizeof(UnorderedAccessViewTexture3D), "Dummy must encapsulates all sub structures");
			}

		private:
			struct ViewMetaDataDummy
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
			ViewRenderTargetType	ResourceViewRenderTargetType	= ViewRenderTargetType::VIEW_RENDER_TARGET_UNKNOWN;
			ViewDepthStencilType	ResourceViewDepthStencilType	= ViewDepthStencilType::VIEW_DEPTH_STENCIL_UNKNOWN;

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
				_In_ const ViewShaderResourceType& InViewShaderResourceType,
				_In_ const ViewPlane& InViewPlane = ViewPlane::VIEW_PLANE_DEFAULT
			)
				: ViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					InFormat,
					ViewType::VIEW_SHADER_RESOURCE
				)
				, _ViewPlane(InViewPlane)
			{
				ResourceViewShaderResourceType = InViewShaderResourceType;
			}

			const ViewPlane& GetViewPlane() const { return _ViewPlane; }

		private:
			ViewPlane _ViewPlane = ViewPlane::VIEW_PLANE_DEFAULT;
		};
		
		struct ShaderResourceViewStructuredBufferCreateInformation : public ShaderResourceViewCreateInformation
		{
			ShaderResourceViewStructuredBufferCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData
			)
				: ShaderResourceViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					Format::FORMAT_UNKNOWN,
					ViewShaderResourceType::VIEW_SHADER_RESOURCE_BUFFER
				)
			{
			}
		};

		struct ShaderResourceViewAccelerationStructureCreateInformation : public ShaderResourceViewCreateInformation
		{
			ShaderResourceViewAccelerationStructureCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource
			)
				: ShaderResourceViewCreateInformation(
					InContext,
					InResource,
					ViewMetaData(),
					Format::FORMAT_UNKNOWN,
					ViewShaderResourceType::VIEW_SHADER_RESOURCE_RAYTRACING_ACCELERATION_STRUCTURE
				)
			{
			}
		};

		struct UnorderedAccessViewCreateInformation : public ViewCreateInformation
		{
			UnorderedAccessViewCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData,
				_In_ const Format& InFormat,
				_In_ const ViewUnorderedAccessType& InViewUnorderedAccessType
			)
				: ViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					InFormat,
					ViewType::VIEW_UNORDERED_ACCESS
				)
			{
				ResourceViewUnorderedAccessType = InViewUnorderedAccessType;
			}
		};

		struct DepthStencilViewCreateInformation : public ViewCreateInformation
		{
			DepthStencilViewCreateInformation(
				_In_ GraphicsContext& InContext,
				_In_ Resource* InResource,
				_In_ const ViewMetaData& InViewMetaData,
				_In_ const Format& InFormat,
				_In_ const ViewDepthStencilType& InViewDepthStencilType
			)
				: ViewCreateInformation(
					InContext,
					InResource,
					InViewMetaData,
					InFormat,
					ViewType::VIEW_DEPTH_STENCIL
				)
			{
				ResourceViewDepthStencilType = InViewDepthStencilType;
			}
		};

		class View
		{
		public:

			static std::unordered_set<View*> ViewHistory;

			virtual ~View();

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
