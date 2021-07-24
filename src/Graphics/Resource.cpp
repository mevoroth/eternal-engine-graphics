#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		constexpr ResourceType operator|(const ResourceType& InLeftResourceType, const ResourceType& InRightResourceType)
		{
			return static_cast<ResourceType>(
				static_cast<uint32_t>(InLeftResourceType) | static_cast<uint32_t>(InRightResourceType)
			);
		}

		ResourceType operator&(const ResourceType& InLeftResourceType, const ResourceType& InRightResourceType)
		{
			return static_cast<ResourceType>(
				static_cast<uint32_t>(InLeftResourceType) & static_cast<uint32_t>(InRightResourceType)
				);
		}

		//////////////////////////////////////////////////////////////////////////

		Resource::Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceType& InResourceType)
			: _ResourceCreateInformation(InResourceCreateInformation)
			, _ResourceType(InResourceType)
		{
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

		const ResourceDimension& Resource::GetResourceDimension() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Dimension;
		}

		uint32_t Resource::GetMIPLevels() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.MIPLevels;
		}

		uint32_t Resource::GetArraySize() const
		{
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

		uint32_t Resource::GetBufferSize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.Size;
		}

		uint32_t Resource::GetBufferStride() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.Stride;
		}

		const Format& Resource::GetFormat() const
		{
			return GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE ? _ResourceCreateInformation.TextureInformation.ResourceFormat : _ResourceCreateInformation.BufferInformation.ResourceFormat;
		}
	}
}
