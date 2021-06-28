#include "d3d12/D3D12InputLayout.hpp"

#include "d3d12/D3D12Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr char* D3D12_INPUT_SEMANTICS[] =
		{
			"SV_Position",
			"COLOR",
			"NORMAL",
			"TANGENT",
			"BINORMAL",
			"TEXCOORD",
			"SV_Position"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_INPUT_SEMANTICS) == static_cast<int32_t>(VertexDataType::VERTEX_DATA_TYPE_COUNT), "Mismatch between abstraction and d3d12 input semantics");

		D3D12InputLayout::D3D12InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount)
		{
			_Input.resize(DataTypeCount);
			for (uint32_t DataTypeIndex = 0; DataTypeIndex < DataTypeCount; ++DataTypeIndex)
			{
				uint32_t DataTypeInt = static_cast<uint32_t>(DataType[DataTypeIndex]);

				_Input[DataTypeIndex].SemanticName			= D3D12_INPUT_SEMANTICS[DataTypeInt];
				_Input[DataTypeIndex].SemanticIndex			= 0;
				_Input[DataTypeIndex].Format				= ConvertVertexDataTypeToD3D12Format(DataType[DataTypeIndex]).Format;
				_Input[DataTypeIndex].InputSlot				= 0;
				_Input[DataTypeIndex].AlignedByteOffset		= D3D12_APPEND_ALIGNED_ELEMENT;
				_Input[DataTypeIndex].InputSlotClass		= D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				_Input[DataTypeIndex].InstanceDataStepRate	= 0;
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
