#include "d3d11/D3D11InputLayout.hpp"

#include "d3d11/D3D11Renderer.hpp"
#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

static const char* D3D11_INPUT_SEMANTICS[]
{
	"SV_Position",
	"COLOR",
	"NORMAL",
	"TANGENT",
	"TEXCOORD"
};

static const DXGI_FORMAT D3D11_INPUT_FORMATS[]
{
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
};

D3D11InputLayout::D3D11InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t Size)
	: _InputLayout(0)
{
	ETERNAL_ASSERT(D3D11Renderer::Get());
	ETERNAL_ASSERT(dynamic_cast<D3D11Renderer*>(D3D11Renderer::Get())->GetDevice());
	
	for (uint32_t DataTypeIndex = 0; DataTypeIndex < Size; ++DataTypeIndex)
	{
		_AddInputDesc(D3D11_INPUT_SEMANTICS[DataType[DataTypeIndex]], D3D11_INPUT_FORMATS[DataType[DataTypeIndex]]);
	}
}

void D3D11InputLayout::_AddInputDesc(_In_ const char* Semantic, _In_ const DXGI_FORMAT& Format)
{
	D3D11_INPUT_ELEMENT_DESC InputTemp;

	InputTemp.SemanticName = Semantic;
	InputTemp.SemanticIndex = 0;
	InputTemp.Format = Format;
	InputTemp.InputSlot = 0;
	InputTemp.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	InputTemp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	InputTemp.InstanceDataStepRate = 0;
	
	_Input.push_back(InputTemp);
}

ID3D11InputLayout* D3D11InputLayout::GetD3D11InputLayout() const
{
	return _InputLayout;
}
