#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		constexpr uint32_t TextureCreateInformation::ComponentCount;
		constexpr float TextureCreateInformation::DefaultClearValueZero[ComponentCount];
		constexpr float TextureCreateInformation::DefaultClearValueOne[ComponentCount];
		constexpr float TextureCreateInformation::DefaultClearValueDepth[ComponentCount];

		//////////////////////////////////////////////////////////////////////////

		constexpr ResourceType operator|(_In_ const ResourceType& InLeftResourceType, _In_ const ResourceType& InRightResourceType)
		{
			return static_cast<ResourceType>(
				static_cast<uint32_t>(InLeftResourceType) | static_cast<uint32_t>(InRightResourceType)
			);
		}

		ResourceType operator&(_In_ const ResourceType& InLeftResourceType, _In_ const ResourceType& InRightResourceType)
		{
			return static_cast<ResourceType>(
				static_cast<uint32_t>(InLeftResourceType) & static_cast<uint32_t>(InRightResourceType)
			);
		}

		//////////////////////////////////////////////////////////////////////////

		std::unordered_set<Resource*> Resource::ResourceHistory;

		Resource::Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceType& InResourceType)
			: _ResourceType(InResourceType)
			, _ResourceCreateInformation(InResourceCreateInformation)
		{
			ETERNAL_ASSERT(_ResourceCreateInformation.Name.size() > 0);
			ResourceHistory.insert(this);
		}

		Resource::~Resource()
		{
			ResourceHistory.erase(this);
		}

		ResourceType Resource::GetResourceType() const
		{
			static constexpr ResourceType Texture_Buffer_Type = (ResourceType::RESOURCE_TYPE_BUFFER | ResourceType::RESOURCE_TYPE_TEXTURE);
			ResourceType FundamentalType = _ResourceType & Texture_Buffer_Type;
			ETERNAL_ASSERT(FundamentalType != Texture_Buffer_Type); // Cannot be both
			return FundamentalType;
		} 

		const float* Resource::GetClearValue() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.ClearValue;
		}

		uint8_t Resource::GetStencilClearValue() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.StencilClearValue;
		}

		void Resource::SetResourceState(_In_ const TransitionState& InTransitionState)
		{
			_ResourceCreateInformation.ResourceState = InTransitionState;
			for (uint32_t SubResource = 0; SubResource < _SubResourceStates.size(); ++SubResource)
				_SubResourceStates[SubResource] = InTransitionState;
		}

		void Resource::SetSubResourceState(_In_ uint32_t InSubResourceIndex, _In_ const TransitionState& InTransitionState)
		{
			ETERNAL_ASSERT(InSubResourceIndex < _SubResourceStates.size());
			_SubResourceStates[InSubResourceIndex] = InTransitionState;
		}

		const TransitionState& Resource::GetResourceState(_In_ uint32_t InSubResourceIndex) const
		{
			ETERNAL_ASSERT(InSubResourceIndex < _SubResourceStates.size());
			return _SubResourceStates[InSubResourceIndex];
		}

		const ResourceDimension& Resource::GetResourceDimension() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Dimension;
		}

		uint32_t Resource::GetWidth() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Width;
		}

		uint32_t Resource::GetHeight() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Height;
		}

		uint32_t Resource::GetDepth() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			switch (GetResourceDimension())
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
				return 6;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			default:
				return 1;
			}
		}

		uint32_t Resource::GetMIPLevels() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.MIPLevels;
		}

		uint32_t Resource::GetArraySize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			switch (GetResourceDimension())
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
				return _ResourceCreateInformation.TextureInformation.Height;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize / 6;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
			default:
				return 1;
				break;
			}
		}

		uint32_t Resource::GetDepthOrArraySize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			switch (GetResourceDimension())
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
				return _ResourceCreateInformation.TextureInformation.Height;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			default:
				return 1;
				break;
			}
		}

		uint32_t Resource::GetBufferSize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.Stride * _ResourceCreateInformation.BufferInformation.ElementCount;
		}

		uint32_t Resource::GetBufferStride() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.Stride;
		}

		uint32_t Resource::GetElementCount() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.ElementCount;
		}

		const Format& Resource::GetFormat() const
		{
			return GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE ? _ResourceCreateInformation.TextureInformation.ResourceFormat : _ResourceCreateInformation.BufferInformation.ResourceFormat;
		}

		const std::string& Resource::GetResourceName() const
		{
			return _ResourceCreateInformation.Name;
		}

		bool Resource::IsBackBuffer() const
		{
			return GetRawResourceType() == ResourceType::RESOURCE_TYPE_BACK_BUFFER;
		}
	}
}
