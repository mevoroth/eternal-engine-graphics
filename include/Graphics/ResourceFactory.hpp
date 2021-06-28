#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
		struct TextureResourceCreateInformation;
		struct BufferResourceCreateInformation;

		Resource* CreateTexture(_In_ const TextureResourceCreateInformation& InResourceCreateInformation);
		Resource* CreateBuffer(_In_ const BufferResourceCreateInformation& InResourceCreateInformation);
	}
}
