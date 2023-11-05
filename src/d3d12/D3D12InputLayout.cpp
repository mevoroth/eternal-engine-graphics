#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12InputLayout.hpp"

#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Format.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		static constexpr char* D3D12_INPUT_SEMANTICS[] =
		{
			"POSITION",
			"COLOR",
			"NORMAL",
			"TANGENT",
			"BINORMAL",
			"TEXCOORD"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_INPUT_SEMANTICS) == static_cast<int32_t>(VertexDataType::VERTEX_DATA_TYPE_COUNT), "Mismatch between abstraction and d3d12 input semantics");

		static const D3D12_INPUT_CLASSIFICATION ConvertVertexStreamFrequenceToD3D12InputClassification(_In_ const VertexStreamFrequency& InVertexStreamFrequency)
		{
			return static_cast<D3D12_INPUT_CLASSIFICATION>(InVertexStreamFrequency);
		}

		D3D12InputLayout::D3D12InputLayout(_In_ const vector<VertexStreamBase>& InVertexStreams)
			: InputLayout(InVertexStreams)
		{
			if (InVertexStreams.size() == 0)
				return;

			std::array<uint32_t, static_cast<uint32_t>(VertexDataType::VERTEX_DATA_TYPE_COUNT)> SemanticTypesIndex;
			SemanticTypesIndex.fill(0);

			uint32_t TotalElementsCount = 0;
			for (uint32_t VertexStreamIndex = 0; VertexStreamIndex < InVertexStreams.size(); ++VertexStreamIndex)
				TotalElementsCount += static_cast<uint32_t>(InVertexStreams[VertexStreamIndex].GetElements().size());

			_InputElements.reserve(TotalElementsCount);

			for (uint32_t VertexStreamIndex = 0; VertexStreamIndex < InVertexStreams.size(); ++VertexStreamIndex)
			{
				const vector<VertexStreamElement>& Elements = InVertexStreams[VertexStreamIndex].GetElements();

				for (uint32_t ElementIndex = 0; ElementIndex < Elements.size(); ++ElementIndex)
				{
					_InputElements.push_back({});
					D3D12_INPUT_ELEMENT_DESC& D3D12InputElementDesc = _InputElements.back();

					uint32_t DataTypeUInt = static_cast<uint32_t>(Elements[ElementIndex].Type);

					D3D12InputElementDesc.SemanticName			= D3D12_INPUT_SEMANTICS[DataTypeUInt];
					D3D12InputElementDesc.SemanticIndex			= SemanticTypesIndex[DataTypeUInt]++;
					D3D12InputElementDesc.Format				= ConvertFormatToD3D12Format(Elements[ElementIndex].VertexFormat).Format;
					D3D12InputElementDesc.InputSlot				= VertexStreamIndex;
					D3D12InputElementDesc.AlignedByteOffset		= D3D12_APPEND_ALIGNED_ELEMENT;
					D3D12InputElementDesc.InputSlotClass		= ConvertVertexStreamFrequenceToD3D12InputClassification(InVertexStreams[VertexStreamIndex].GetVertexStreamFrequency());
					D3D12InputElementDesc.InstanceDataStepRate	= InVertexStreams[VertexStreamIndex].GetVertexStreamFrequency() == VertexStreamFrequency::VERTEX_STREAM_FREQUENCY_VERTEX ? 0 : InVertexStreams[VertexStreamIndex].GetStreamStride();
				}
			}
		}

		const D3D12Format& ConvertVertexDataTypeToD3D12Format(const VertexDataType& InVertexDataType)
		{
			return ConvertFormatToD3D12Format(
				ConvertVertexDataTypeToFormat(InVertexDataType)
			);
		}
	}
}

#endif
