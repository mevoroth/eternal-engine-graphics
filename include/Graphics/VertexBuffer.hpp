#pragma once

#include "Graphics/MultiBuffered.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/View.hpp"
#include "Graphics/ViewFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename VertexBufferType, uint32_t VerticesCount = 16384>
		struct VertexBuffer
		{
			VertexBuffer(_In_ GraphicsContext& InContext, _In_ const char* InBufferName)
			{
				ResourceBuffer = CreateMultiBufferedBuffer(
					InContext,
					BufferResourceCreateInformation(
						InContext.GetDevice(),
						InBufferName,
						VertexBufferCreateInformation(
							sizeof(VertexBufferType),
							VerticesCount
						),
						ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
					)
				);
			}

			~VertexBuffer()
			{
				DestroyMultiBufferedResource(ResourceBuffer);
			}

			Resource* GetResource()
			{
				return *ResourceBuffer;
			}

			operator Resource* ()
			{
				return *ResourceBuffer;
			}

			operator Resource& ()
			{
				return *ResourceBuffer;
			}

			MultiBuffered<Resource>* ResourceBuffer = nullptr;
		};
	}
}
