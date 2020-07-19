#include "d3d11/D3D11DepthStencil.hpp"

#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"
#include "d3d11/D3D11Renderer.hpp"

#include <d3d11.h>

using namespace Eternal::Graphics;

D3D11DepthStencil::D3D11DepthStencil(_In_ const DepthTest& DepthTestState, _In_ const StencilTest& StencilState)
	: DepthStencil(DepthTestState, StencilState)
{
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	
	// Depth Testing
	DepthStencilDesc.DepthEnable = DepthTestState.IsEnabled() ? TRUE : FALSE;
	DepthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)(DepthTestState.GetComparisonOperator() + 1);
	DepthStencilDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)DepthTestState.GetMask();

	// Stencil Testing
	DepthStencilDesc.StencilEnable = StencilState.IsEnabled() ? TRUE : FALSE;
	DepthStencilDesc.StencilReadMask = (UINT8)StencilState.GetReadMask();
	DepthStencilDesc.StencilWriteMask = (UINT8)StencilState.GetWriteMask();
	DepthStencilDesc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)(StencilState.GetFront().Fail + 1);
	DepthStencilDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)(StencilState.GetFront().FailDepth + 1);
	DepthStencilDesc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)(StencilState.GetFront().Pass + 1);
	DepthStencilDesc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)(StencilState.GetFront().ComparisonOp + 1);
	DepthStencilDesc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)(StencilState.GetBack().Fail + 1);
	DepthStencilDesc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)(StencilState.GetBack().FailDepth + 1);
	DepthStencilDesc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)(StencilState.GetBack().Pass + 1);
	DepthStencilDesc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)(StencilState.GetBack().ComparisonOp + 1);

	HRESULT hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateDepthStencilState(&DepthStencilDesc, &_DepthStencilState);
	ETERNAL_ASSERT(hr == S_OK);
	ETERNAL_ASSERT(_DepthStencilState);
}

D3D11DepthStencil::~D3D11DepthStencil()
{
	_DepthStencilState->Release();
	_DepthStencilState = nullptr;
}
