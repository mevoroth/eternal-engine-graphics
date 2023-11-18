#pragma once

#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullResource final : public Resource
		{
		public:

			NullResource(_In_ const TextureResourceCreateInformation& InResourceCreateInformation)
				: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_TEXTURE)
			{
			}
			NullResource(_In_ const BufferResourceCreateInformation& InResourceCreateInformation)
				: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_BUFFER)
			{
			}

			virtual void Unmap(_In_ const MapRange& InMapRange) override final {}
			virtual TextureToBufferMemoryFootprint GetTextureToBufferMemoryFootprint(_In_ Device& InDevice) const override final
			{
				return TextureToBufferMemoryFootprint();
			}

		protected:

			virtual void* Map(_In_ const MapRange& InMapRange) override final
			{
				return nullptr;
			}

		};
	}
}
