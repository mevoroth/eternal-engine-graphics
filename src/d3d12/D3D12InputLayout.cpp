#include "d3d12/D3D12InputLayout.hpp"

using namespace Eternal::Graphics;

static const char* D3D12_INPUT_SEMANTICS[]
{
	"SV_Position",
	"COLOR",
	"NORMAL",
	"TANGENT",
	"TEXCOORD"
};

static const DXGI_FORMAT D3D12_INPUT_FORMATS[]
{
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
};

D3D12InputLayout::D3D12InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount)
{
	for (uint32_t DataTypeIndex = 0; DataTypeIndex < DataTypeCount; ++DataTypeIndex)
	{
		_AddInputDesc(D3D12_INPUT_SEMANTICS[DataType[DataTypeIndex]], D3D12_INPUT_FORMATS[DataType[DataTypeIndex]]);
	}
}

void D3D12InputLayout::_AddInputDesc(_In_ const char* Semantic, _In_ const DXGI_FORMAT& Format)
{
	D3D12_INPUT_ELEMENT_DESC InputTemp;

	InputTemp.SemanticName = Semantic;
	InputTemp.SemanticIndex = 0;
	InputTemp.Format = Format;
	InputTemp.InputSlot = 0;
	InputTemp.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	InputTemp.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	InputTemp.InstanceDataStepRate = 0;

	_Input.push_back(InputTemp);
}
