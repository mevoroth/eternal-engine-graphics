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
		template<typename ConstantBufferType>
		struct ConstantBuffer
		{
			ConstantBuffer(_In_ GraphicsContext& InContext, _In_ const char* InBufferName)
			{
				ResourceBuffer = CreateMultiBufferedBuffer(
					InContext,
					BufferResourceCreateInformation(
						InContext.GetDevice(),
						InBufferName,
						BufferCreateInformation(
							Format::FORMAT_UNKNOWN,
							BufferResourceUsage::BUFFER_RESOURCE_USAGE_CONSTANT_BUFFER,
							sizeof(ConstantBufferType)
						),
						ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
					)
				);

				ViewMetaData MetaData;
				MetaData.ConstantBufferView.BufferSize = sizeof(ConstantBufferType);
				ResourceView = CreateMultiBufferedConstantBufferView(
					*ResourceBuffer,
					ConstantBufferViewCreateInformation(
						InContext,
						*ResourceBuffer,
						MetaData
					)
				);
			}

			~ConstantBuffer()
			{
				DestroyMultiBufferedResource(ResourceBuffer);
				DestroyMultiBufferedView(ResourceView);
			}

			View* GetView()
			{
				return *ResourceView;
			}

			Resource* GetResource()
			{
				return *ResourceBuffer;
			}

			MultiBuffered<Resource>*	ResourceBuffer	= nullptr;
			MultiBuffered<View>*		ResourceView	= nullptr;
		};
	}
}
