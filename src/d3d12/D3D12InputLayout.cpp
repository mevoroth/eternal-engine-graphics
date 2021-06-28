#include "d3d12/D3D12InputLayout.hpp"

#include "d3d12/D3D12Format.hpp"

using namespace Eternal::Graphics;

static const char* D3D12_INPUT_SEMANTICS[]
{
	"SV_Position",
	"COLOR",
	"NORMAL",
	"TANGENT",
	"BINORMAL",
	"TEXCOORD",
	"SV_Position"
};

D3D12InputLayout::D3D12InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount)
{
	_Input.resize(DataTypeCount);
	for (uint32_t DataTypeIndex = 0; DataTypeIndex < DataTypeCount; ++DataTypeIndex)
	{
		_Input[DataTypeIndex].SemanticName			= D3D12_INPUT_SEMANTICS[DataType[DataTypeIndex]];
		_Input[DataTypeIndex].SemanticIndex			= 0;
		_Input[DataTypeIndex].Format				= D3D12_FORMATS[static_cast<int32_t>(VERTEX_FORMATS[DataType[DataTypeIndex]])].Format;
		_Input[DataTypeIndex].InputSlot				= 0;
		_Input[DataTypeIndex].AlignedByteOffset		= D3D12_APPEND_ALIGNED_ELEMENT;
		_Input[DataTypeIndex].InputSlotClass		= D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		_Input[DataTypeIndex].InstanceDataStepRate	= 0;
	}
}
