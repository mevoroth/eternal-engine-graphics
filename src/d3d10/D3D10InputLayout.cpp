#include "d3d10/D3D10InputLayout.hpp"

#include "d3d10/D3D10Renderer.hpp"

using namespace Eternal::Graphics;

static const char* D3D10_INPUT_SEMANTICS[]
{
	"SV_Position",
	"COLOR",
	"NORMAL",
	"TANGENT",
	"TEXCOORD"
};

static const DXGI_FORMAT D3D10_INPUT_FORMATS[]
{
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
};

D3D10InputLayout::D3D10InputLayout(_In_ const VertexDataType& dataType)
	: _inputLayout(0)
{
	assert(Renderer::Get());
	assert(dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice());

	for (int i = 0; i < VERTEX_DATA_TYPE_COUNT; ++i)
	{
		if (dataType & (1 << i))
		{
			_AddInputDesc(D3D10_INPUT_SEMANTICS[i], D3D10_INPUT_FORMATS[i]);
		}
	}
}

void D3D10InputLayout::_AddInputDesc(_In_ const char* semantic, _In_ const DXGI_FORMAT& format)
{
	D3D10_INPUT_ELEMENT_DESC inputTemp;
	memset(&inputTemp, 0x0, sizeof(D3D10_INPUT_ELEMENT_DESC));
	inputTemp.SemanticName = semantic;
	inputTemp.SemanticIndex = 0;
	inputTemp.Format = format;
	inputTemp.InputSlot = 0;
	inputTemp.AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	inputTemp.InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	inputTemp.InstanceDataStepRate = 0;
	_input.push_back(inputTemp);
}

ID3D10InputLayout* D3D10InputLayout::GetD3D10InputLayout() const
{
	return _inputLayout;
}