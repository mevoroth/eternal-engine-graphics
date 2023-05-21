#pragma once

#include "Graphics/MultiBuffered.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename StructuredBufferType>
		struct StructuredBuffer
		{
			StructuredBuffer(_In_ GraphicsContext& InContext, _In_ const char* InBufferName, _In_ uint32_t InElementsCount)
			{
				ResourceBuffer = CreateMultiBufferedBuffer(
					InContext,
					BufferResourceCreateInformation(
						InContext.GetDevice(),
						InBufferName,
						BufferCreateInformation(
							Format::FORMAT_UNKNOWN,
							BufferResourceUsage::BUFFER_RESOURCE_USAGE_STRUCTURED_BUFFER,
							sizeof(StructuredBufferType),
							InElementsCount
						),
						ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
					)
				);

				ViewMetaData MetaData;
				MetaData.ShaderResourceViewBuffer.NumElements			= InElementsCount;
				MetaData.ShaderResourceViewBuffer.StructureByteStride	= sizeof(StructuredBufferType);
				ResourceView = CreateMultiBufferedShaderResourceView(
					*ResourceBuffer,
					ShaderResourceViewStructuredBufferCreateInformation(
						InContext,
						*ResourceBuffer,
						MetaData
					)
				);
			}

			~StructuredBuffer()
			{
				DestroyMultiBufferedResource(ResourceBuffer);
				DestroyMultiBufferedView(ResourceView);
			}

			View* GetView()
			{
				return *ResourceView;
			}

			MultiBuffered<Resource>* ResourceBuffer = nullptr;
			MultiBuffered<View>* ResourceView = nullptr;
		};
	}
}
