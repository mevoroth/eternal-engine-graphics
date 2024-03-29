#pragma once

#include "Graphics/MultiBuffered.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
		struct TextureResourceCreateInformation;
		struct BufferResourceCreateInformation;

		Resource* CreateTexture(_In_ const TextureResourceCreateInformation& InResourceCreateInformation);
		Resource* CreateBuffer(_In_ const BufferResourceCreateInformation& InResourceCreateInformation);
		MultiBuffered<Resource>* CreateMultiBufferedBuffer(_In_ GraphicsContext& InContext, _In_ const BufferResourceCreateInformation& InResourceCreateInformation);
		void DestroyResource(_Inout_ Resource*& InOutResource);
		void DestroyMultiBufferedResource(_Inout_ MultiBuffered<Resource>*& InOutMultiBufferedResource);
	}
}
