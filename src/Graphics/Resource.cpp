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
	}
}
